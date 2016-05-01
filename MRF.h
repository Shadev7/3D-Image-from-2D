#ifndef __MRF_H__
#define __MRF_H__
#define cimg_verbosity 3
#include <CImg.h>

class MRF {
	int num_states;
	cimg_library::CImg<double> prev_msg, cur_msg;

	cimg_library::CImg<double> get_states(const cimg_library::CImg<double>&,
						const cimg_library::CImg<double>&);
	void init();
public:
	MRF(int width, int height, int num_states);
	~MRF(){}

	cimg_library::CImg<double> solve(const cimg_library::CImg<double>& img);
};

#endif
