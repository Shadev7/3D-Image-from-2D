#include <iostream>

#include <MRFSegmenter.h>
#include <NaiveSegmenter.h>
#include <MRF.h>

using namespace cimg_library;

CImg<double> MRFSegmenter::segment(const CImg<double>& img,
		const std::vector<Point>& fg, const std::vector<Point>& bg) const{
	MRF mrf(img.width(), img.height(), 2);
	return mrf.solve(NaiveSegmenter::get_cost_image(img, fg, bg));
}
