#ifndef __MRF_SEGMENTER_H__
#define __MRF_SEGMENTER_H__

#include <BaseSegmenter.h>

class MRFSegmenter: public BaseSegmenter {
public:
	MRFSegmenter() {}
	~MRFSegmenter() {}

	cimg_library::CImg<double> segment(
			const cimg_library::CImg<double>&,
			const std::vector<Point>&,
			const std::vector<Point>&) const;
};

#endif
