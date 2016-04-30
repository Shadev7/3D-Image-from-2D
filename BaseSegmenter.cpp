#include <cassert>
#include <cmath>

#include <BaseSegmenter.h>

using namespace cimg_library;

CImg<double> BaseSegmenter::segment(const CImg<double>&img,
			const std::vector<Point>& fg, const std::vector<Point>& bg) const {
	CImg<double> result(img.width(), img.height());

	for(int i=0; i<img.height(); i++)
		for(int j=0; j<img.width(); j++)
			result(j, i, 0, 0) = rand() % 2;

	return result;
}


std::pair<CImg<double>, CImg<double> > BaseSegmenter::get_images(
			const CImg<double> &img, const CImg<double> &labels) {
	assert(img.height() == labels.height());
	assert(img.width() == labels.width());

	CImg<double> img_fg = img, img_bg = img;

	for(int i=0; i<labels.height(); i++) {
		for(int j=0; j<labels.width(); j++) {
			if(labels(j,i) == 0)
				img_fg(j,i,0,0) = img_fg(j,i,0,1) = img_fg(j,i,0,2) = 0;
			else if(labels(j,i) == 1)
				img_bg(j,i,0,0) = img_bg(j,i,0,1) = img_bg(j,i,0,2) = 0;
			else
				assert(0);
		}
	}
	return std::make_pair(img_fg, img_bg);
}

std::pair<std::vector<Point>, std::vector<Point> > BaseSegmenter::get_bgfg(
			const CImg<double>& seeds_rgb) {
	std::vector<Point> fg_pixels, bg_pixels;
	for(int i=0; i<seeds_rgb.height(); i++) {
		for(int j=0; j<seeds_rgb.width(); j++) {
			// blue --> foreground
			if(std::max(seeds_rgb(j, i, 0, 0), seeds_rgb(j, i, 0, 1)) < 100 && 
						seeds_rgb(j, i, 0, 2) > 100)
				fg_pixels.push_back(Point(j, i));

			// red --> background
			if(std::max(seeds_rgb(j, i, 0, 2), seeds_rgb(j, i, 0, 1)) < 100 &&
						seeds_rgb(j, i, 0, 0) > 100)
				bg_pixels.push_back(Point(j, i));
		}
	}
	return std::make_pair(fg_pixels, bg_pixels);
}

