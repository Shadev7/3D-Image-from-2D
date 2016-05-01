#ifndef __BASE_SEGMENTER_H__
#define __BASE_SEGMENTER_H__

#include <vector>
#include <algorithm>

#include <CImg.h>
#include <Point.h>

class BaseSegmenter {
public:
	BaseSegmenter() {}

	virtual cimg_library::CImg<double> segment(
			const cimg_library::CImg<double>&,
			const std::vector<Point>& fg, 
			const std::vector<Point>& bg) const;

	static std::pair<cimg_library::CImg<double>, cimg_library::CImg<double> > 
			get_images(const cimg_library::CImg<double>&,
						const cimg_library::CImg<double>&);

	static std::pair<std::vector<Point>, std::vector<Point> > get_bgfg(
			const cimg_library::CImg<double>& img);

	virtual ~BaseSegmenter(){}
};

#endif
