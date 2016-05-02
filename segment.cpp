// Skeleton code for B657 A4 Part 2.
// D. Crandall
//
//
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <CImg.h>
#include <assert.h>

#include <NaiveSegmenter.h>
#include <MRFSegmenter.h>
#include <Matrix.h>
#include <Normal.h>

using namespace cimg_library;
using namespace std;


// Take in an input image and a binary segmentation map. Use the segmentation map to split the 
//  input image into foreground and background portions, and then save each one as a separate image.
//
// You'll just need to modify this to additionally output a disparity map.
//
int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		cerr << "usage: " << argv[0] << " image_file seeds_file" << endl;
		return 1;
	}

	string input_filename1 = argv[1], input_filename2 = argv[2];

	// read in images and gt
	CImg<double> image_rgb(input_filename1.c_str());
	CImg<double> seeds(input_filename2.c_str());

	// figure out seed points 
	std::pair<vector<Point>, vector<Point> > fgbg(BaseSegmenter::get_bgfg(seeds));
	vector<Point> fg_pixels = fgbg.first, bg_pixels = fgbg.second;
	
	// do naive segmentation
	NaiveSegmenter naive;
	CImg<double> labels = naive.segment(image_rgb, fg_pixels, bg_pixels);

	std::pair<CImg<double>, CImg<double> > pairs = 
			BaseSegmenter::get_images(image_rgb, labels);

	pairs.first.get_normalize(0, 255).save(
			(input_filename1 + "-naive_segment_result_fg.png").c_str());
	pairs.second.get_normalize(0, 255).save(
			(input_filename1 + "-naive_segment_result_bg.png").c_str());
	labels.get_normalize(0, 255).save(
			(input_filename1 + "-naive_segment_result_disparity.png").c_str());

	// do MRF segmentation
	MRFSegmenter mrf;
	labels = mrf.segment(image_rgb, fg_pixels, bg_pixels);

	pairs = BaseSegmenter::get_images(image_rgb, labels);

	pairs.first.get_normalize(0, 255).save(
			(input_filename1 + "-mrf_segment_result_fg.png").c_str());
	pairs.second.get_normalize(0, 255).save(
			(input_filename1 + "-mrf_segment_result_bg.png").c_str());
	labels.get_normalize(0, 255).save(
			(input_filename1 + "-mrf_segment_result_disparity.png").c_str());

	return 0;
}
