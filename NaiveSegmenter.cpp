#include <cmath>
#include <iostream>

#include <NaiveSegmenter.h>
#include <Normal.h>
#include <Config.h>

using namespace cimg_library;


double NaiveSegmenter::cost(double r, double g, double b,
			const Matrix& mean, const SqMatrix& cov) {
	Normal n(mean, cov);
	Matrix m(3, 1);
	m(0, 0) = r;
	m(1, 0) = g;
	m(2, 0) = b;
	return -std::log(n.eval(m));
}

CImg<double> NaiveSegmenter::segment(const CImg<double>&img,
			const std::vector<Point>& fg, const std::vector<Point>& bg) const {
	CImg<double> result(img.width(), img.height());
	double constant_cost = config.get<double>("naive.segmenter.bgcost");

	Matrix mean(3, 1);
	SqMatrix var(3);

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

	for(int i=0; i<img.height(); i++) {
		for(int j=0; j<img.width(); j++) {
			if (std::find(fg.begin(), fg.end(), Point(j, i)) != fg.end()) {
				result(j, i, 0, 0) = 0;
			}
			else if (std::find(bg.begin(), bg.end(), Point(j, i)) != bg.end()) {
				result(j, i, 0, 0) = 1;
			} else {
				double normal_cost = cost(
						img(j, i, 0, 0), img(j, i, 0, 1), img(j, i, 0, 2),
						mean, var);
				result(j, i, 0, 0) = normal_cost > constant_cost;
			}
		}
	}
	return result;
}
