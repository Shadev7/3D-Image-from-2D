#include <cmath>
#include <iostream>
#include <limits>

#include <NaiveSegmenter.h>
#include <Normal.h>
#include <Config.h>

using namespace cimg_library;

CImg<double> NaiveSegmenter::segment(const CImg<double>&img,
			const std::vector<Point>& fg, const std::vector<Point>& bg) const {
	CImg<double> result(img.width(), img.height());
	CImg<double> normal_cost = get_cost_image(img, fg, bg);
	
	for(int i=0; i<img.height(); i++) {
		for(int j=0; j<img.width(); j++) {
			result(j, i, 0, 0) = normal_cost(j,i,1,0) < normal_cost(j,i,0,0);
		}
	}
	return result;
}

CImg<double> NaiveSegmenter::get_cost_image(const CImg<double>& img,
		const std::vector<Point>& fg, const std::vector<Point>& bg) {
	Matrix mean(3, 1);
	SqMatrix var(3);
	double bg_cost = config.get<double>("naive.segmenter.bgcost");


	mean(0,0) = mean(1,0) = mean(2,0) = 0.0;
	var(0,0) = var(0, 1) = var(0, 2) = 0.0;
	var(1,0) = var(1, 1) = var(1, 2) = 0.0;
	var(2,0) = var(2, 1) = var(2, 2) = 0.0;

	for (std::vector<Point>::const_iterator it = fg.begin(); it != fg.end(); it++) {
		for (int c = 0; c < 3; c++) {
			mean(c, 0) += img(it->row, it->col, 0, c);
			var(c, c) += std::pow(img(it->row, it->col, 0, c), 2);
		}
	}

	for (int c = 0; c < 3; c++) {
		mean(c, 0) /= (double)fg.size();
		var(c, c) = var(c, c) / (double)fg.size() - std::pow(mean(c, 0), 2);
	}

	Normal n(mean, var);

	CImg<double> result(img.width(), img.height(), 2, 1);
	for(int i=0; i<img.height(); i++) {
		for(int j=0; j<img.width(); j++) {
			if (std::find(fg.begin(), fg.end(), Point(j, i)) != fg.end()) {
				result(j, i, 0, 0) = std::numeric_limits<double>::infinity();
				result(j, i, 1, 0) = 0;
			}
			else if (std::find(bg.begin(), bg.end(), Point(j, i)) != bg.end()) {
				result(j, i, 0, 0) = 0;
				result(j, i, 1, 0) = std::numeric_limits<double>::infinity();
			}
			else {
				Matrix m(3, 1);
				for (int row = 0; row < 3; row++) {
					m(row, 0) = img(j, i, 0, row);
				}
				result(j, i, 1, 0) = -std::log(n.eval(m));
				result(j, i, 0, 0) = bg_cost;
			}
		}
	}
	return result;
}
