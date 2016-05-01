#include <iostream>

#include <MRFSegmenter.h>
#include <NaiveSegmenter.h>

#define DIR_SOUTH 0
#define DIR_NORTH 1 
#define DIR_EAST 2
#define DIR_WEST 3

using namespace cimg_library;

namespace {
	int get_dir(int x1, int y1, int x2, int y2) {
		//(x1, y1): FROM | (x2, y2): TO
		if (x1 == x2)
			return y1 > y2? DIR_SOUTH : DIR_NORTH;
		else if (y1 == y2)
			return x1 > x2? DIR_WEST : DIR_EAST;
		return 0;
	}

	std::vector<Point> get_neighbors(CImg<double> img, int x, int y) {
		std::vector<Point> result;
		if (x + 1 < img.width())
			result.push_back(Point(x+1, y));
		if (x - 1 >= 0)
			result.push_back(Point(x-1, y));
		if (y + 1 < img.height())
			result.push_back(Point(x, y+1));
		if (y - 1 >= 0)
			result.push_back(Point(x, y-1));
		return result;
	}

}

CImg<double> MRFSegmenter::segment(const CImg<double>& img, 
			const std::vector<Point>& fg, const std::vector<Point>& bg) const {
	CImg<double> D(NaiveSegmenter::get_cost_image(img, fg, bg));

	CImg<double> prev_msg(img.width(), img.height(), 4, 2);	//4 neighbors on Z axis
	CImg<double> cur_msg(img.width(), img.height(), 4, 2);	//2 probabilities

	for (int i = 0; i<img.height(); i++) {
		for (int j=0; j<img.width(); j++) {
			for (int n = 0; n<4; n++) {
				prev_msg(j, i, n, 0) = -log(0.5);
				prev_msg(j, i, n, 1) = -log(0.5);
			}
		}
	}
	for (int iter = 0; iter < 10; iter++) {
		std::cout<<"MRF Iteration #"<<iter<<std::endl;
		for (int i=0; i<img.height(); i++) {
			for (int j=0; j<img.width(); j++) {
				std::vector<Point> neighbors(get_neighbors(img, j, i));
				for (std::vector<Point>::iterator it1 = neighbors.begin();
										it1 != neighbors.end(); it1++) {
					/*
					 * Calculation for neighbor it1 for qj = 0 
					 **/

					//Calculation for bg val = 0
					double sum_00 = D(j, i, 0, 0) + 
								(D(j, i, 0, 0) != D(it1->row, it1->col, 0, 0));
					
					//Calculation for bg val = 1
					double sum_10 = D(j, i, 1, 0) +
								(D(j, i, 1, 0) != D(it1->row, it1->col, 0, 0));

					for (std::vector<Point>::iterator it2 = neighbors.begin();
										it2 != neighbors.end(); it2++) {
						if (*it2 == *it1) {
							continue;
						}
						int dir_in = get_dir(it2->row, it2->col, j, i);
						sum_00 += prev_msg(it2->row, it2->col, dir_in, 0);
						sum_10 += prev_msg(it2->row, it2->col, dir_in, 1);
					}
					int dir_out0 = get_dir(j, i, it1->row, it1->col);
					cur_msg(j, i, dir_out0, 0) = std::min(sum_00, sum_10);

					/*
					 * Calculation for neighbor it1 for qj = 1 
					 **/

					//Calculation for bg val = 0
					double sum_01 = D(j, i, 0, 0) + 
								(D(j, i, 0, 0) != D(it1->row, it1->col, 1, 0));
					
					//Calculation for bg val = 1
					double sum_11 = D(j, i, 1, 0) +
								(D(j, i, 1, 0) != D(it1->row, it1->col, 1, 0));

					for (std::vector<Point>::iterator it2 = neighbors.begin();
										it2 != neighbors.end(); it2++) {
						if (*it2 == *it1) {
							continue;
						}
						int dir_in = get_dir(it2->row, it2->col, j, i);
						sum_01 += prev_msg(it2->row, it2->col, dir_in, 0);
						sum_11 += prev_msg(it2->row, it2->col, dir_in, 1);
					}
					int dir_out1 = get_dir(j, i, it1->row, it1->col);
					cur_msg(j, i, dir_out1, 1) = std::min(sum_01, sum_11);
				}
			}
		}
		prev_msg = cur_msg;
	}

	CImg<double> result(img.width(), img.height());

	//Assume coverge. Calculate final probability
	for (int i=0; i<img.height(); i++) {
		for (int j=0; j<img.width(); j++) {
			double cost_0 = D(j, i, 0, 0);
			double cost_1 = D(j, i, 1, 0);

			std::vector<Point> neighbors = get_neighbors(img, j, i);
			for (std::vector<Point>::iterator it = neighbors.begin(); 
							it != neighbors.end(); it++) {
				int dir_in = get_dir(it->row, it->col, j, i);
				cost_0 += cur_msg(j, i, dir_in, 0);
				cost_1 += cur_msg(j, i, dir_in, 1);
			}
			result(j, i, 0, 0) = cost_1 < cost_0;
		}
	}
	return result;
}
