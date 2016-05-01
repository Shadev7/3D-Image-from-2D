#ifndef __NAIVE_SEGMENTER_H__
#define __NAIVE_SEGMENTER_H__

#include <BaseSegmenter.h>
#include <Matrix.h>

class NaiveSegmenter : public BaseSegmenter {
	static double cost(double r, double g, double b, 
			const Matrix& mean, const SqMatrix& cov);
public:
	NaiveSegmenter() {}
	virtual ~NaiveSegmenter(){}

	static cimg_library::CImg<double> get_cost_image(
			const cimg_library::CImg<double>&,
			const std::vector<Point>&, const std::vector<Point>&);

	cimg_library::CImg<double> segment(const cimg_library::CImg<double>&,
			const std::vector<Point>&, const std::vector<Point>&) const;
};

#endif
