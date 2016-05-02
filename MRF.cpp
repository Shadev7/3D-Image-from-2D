#include <vector>
#include <algorithm>
#include <iostream>

#include "MRF.h"
#include "Config.h"
#include "Point.h"

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
		std::cout<<"Invalid direction: "<<x1<<","<<y1<<","<<x2<<","<<y2<<std::endl;
		throw "Invalid Direction.";
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

MRF::MRF(int w, int h, int ns): num_states(ns),
				prev_msg(w, h, 4, ns), cur_msg(w, h, 4, ns) {
	
}

void MRF::init() {
	for (int i = 0; i<prev_msg.height(); i++) {
		for (int j=0; j<prev_msg.width(); j++) {
			for (int n = 0; n<4; n++) {
				for (int s = 0; s<num_states; s++) {
					prev_msg(j, i, n, s) = 0;
					cur_msg(j, i, n, s) = 0;
				}
			}
		}
	}
}

CImg<double> MRF::solve(const CImg<double>& D) {
	init();
	
	int max_iter = config.get<int>("mrf.max_iter");

	for (int iter = 0; iter < max_iter; iter++) {
		std::cout<<"MRF Iteration#"<<iter<<std::endl;

		for (int i=0; i<D.height(); i++) {
			for (int j=0; j<D.width(); j++) {
				std::vector<Point> neighbors(get_neighbors(D, j, i));
				//Iterate over neighbors;
				for (std::vector<Point>::iterator it1 = neighbors.begin();
									it1 != neighbors.end(); it1++) {
					//Calculate probability distribution for neighbor *it1
					for (int s1 = 0; s1 < num_states; s1++) {
						std::vector<double> sum_cost(num_states, 0);
						
						for (int s2 = 0; s2 < num_states; s2++) {
							sum_cost[s2] = D(j, i, s2, 0);
							sum_cost[s2] += s1 != s2;
							for (std::vector<Point>::iterator it2 = neighbors.begin();
										it2 != neighbors.end(); it2++) {
								if (*it1 == *it2) {
									continue;
								}
								int dir_in = get_dir(it2->col, it2->row, j, i);
								sum_cost[s2] += prev_msg(it2->col, it2->row, dir_in, s2);
							}
						}
						int dir_out = get_dir(j, i, it1->col, it1->row);
						cur_msg(j, i, dir_out, s1) = 
									*std::min_element(sum_cost.begin(), sum_cost.end());
					}
				}
			}
		}
		prev_msg = cur_msg;
		get_states(D, cur_msg).get_normalize(0, 255).save("temp.png", iter);
	}
	return get_states(D, cur_msg);
}

CImg<double> MRF::get_states(const CImg<double>& D, const CImg<double>& cur_msg) {
	CImg<double> result(cur_msg.width(), cur_msg.height());
	for (int i=0; i<cur_msg.height(); i++) {
		for (int j=0; j<cur_msg.width(); j++) {
			std::vector<double> sum_cost(num_states, 0);
			for (int s = 0; s < num_states; s++) {
				sum_cost[s] += D(j, i, s, 0);
				std::vector<Point> neighbors(get_neighbors(D, j, i));
				for (std::vector<Point>::iterator it = neighbors.begin(); 
							it != neighbors.end(); it++) {
					int dir = get_dir(it->col, it->row, j, i);
					sum_cost[s] += cur_msg(j, i, dir, s);
				}
			}
			result(j, i) = std::min_element(sum_cost.begin(), sum_cost.end()) 
										- sum_cost.begin();
		}
	}
	return result;
}

