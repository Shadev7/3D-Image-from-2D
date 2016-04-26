// Skeleton code for B657 A4 Part 1.
// D. Crandall
//
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <math.h>
#include <CImg.h>
#include <assert.h>

using namespace cimg_library;
using namespace std;

CImg<double> get_mask(const CImg<int>& depth, int depth) {
	CImg<int> result(depth);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			result.atXYZC(i, j, 0, 0) = result.atXYZC(x, y, 0, 0) == depth;
		}
	}
	return result;
}

std::vector<int> get_unique_values(const CImg<int>& img) {
	std::vector<int> unique;
	
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			int pixel = result.atXYZC(x, y, 0, 0);
			if (std::find(unique.begin(), unique.end(), pixel) == unique.end()) {
				unique.push_back(pixel);
			}
		}
	}
	return unique;
}

CImg<double> multiply(const CImg<double>& input, const CImg<int>& mask) {
	CImg<double> result(input);
	
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (!mask.atXYZC(x, y, 0, 0)) {
				result.atXYZC(x, y, 0, 0) = 0;
			}
		}
	}
	return result;
}

CImg<double> translate_image(const CImg<double>& input, int pixels) {
	CImg<double> result(input.width(), input.height(), 1, 1, 0);

	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (x + pixels < input.width()) {
				result.atXYZC(x, y, 0, 0) = input.atXYZC(x + pixels, y, 0, 0);
			}
		}
	}
	return result;
}

CImg<double> depth_shift_image(const CImg<double>& input, const CImg<int>& depth) {
	CImg<double> result(input.width(), input.height(), 1, 1, 0);
	std::vector<int> unique_depths(get_unique_values(depth));
	std::sort(unique_depths.begin(), unique_depths.end());
	for (std::vector<int>::iterator it = unique_depths.begin(); it != unique_depths.end(); it++) {
		if (!*it)
			continue;

	}
}

CImg<double> get3dImage (CImg<double> image_rgb,CImg<double> image_disp)
{
	
	int wid = image_rgb.width();
	int ht = image_rgb.height();
	//CImg<double> result = image_rgb;
	CImg<double> result(wid, ht, 1, 3, 0);
	for (int i=0; i< wid ; i++)
	{
		for (int j=0; j< ht ; j++)
		{
			//if(image_disp.atXYZC(i,j, 0, 0)!= 0)
			result.atXYZC(i, j, 0, 0) = image_rgb.atXYZC(i,j, 0, 0);
			if (image_disp.atXYZC(i,j, 0, 1)!= 0)
			{
			result.atXYZC(i, j, 0, 1) = image_disp.atXYZC(i,j, 0, 1);
			result.atXYZC(i, j, 0, 2) = image_disp.atXYZC(i,j, 0, 2);
			}
		}
	}
	return result;
}
int main(int argc, char *argv[])
{
  if(argc != 3)
    {
      cerr << "usage: " << argv[0] << " image_file disp_file" << endl;
      return 1;
    }

  string input_filename1 = argv[1], input_filename2 = argv[2];

  // read in images and gt
  CImg<double> image_rgb(input_filename1.c_str());
  CImg<double> image_disp(input_filename2.c_str());
  
  CImg<double> diparityImg = getDiparityImg(image_rgb,image_disp);
  diparityImg.get_normalize(0,255).save((input_filename1 + "-disparity.png").c_str());
  
  CImg<double> image_result = get3dImage (image_rgb,diparityImg);
  image_result.get_normalize(0,255).save((input_filename1 + "-stereogram.png").c_str());

  return 0;
}
