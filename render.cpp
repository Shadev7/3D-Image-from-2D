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

CImg<double> nomalize_depth(const CImg<double>& depth, double normalization_param) {
	CImg<double> result(depth.width(), depth.height(), 1, 1, 0);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			result.atXYZC(x, y, 0, 0) = normalization_param*(depth.atXYZC(x, y, 0, 0) - 127.5)/127.5 + 15;
		}
	}
	// result.display("Normalized disparity map");
	return result;
}

CImg<double> shifted_img(const CImg<double>& input, const CImg<double>& norm_depth, double normalization_param)
{
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	for (int d = 0; d<= 2*normalization_param; d++)
	{
		for (int y = 0; y < result.height(); y++) {
			for (int x = 0; x < result.width(); x++) {
				if (norm_depth.atXYZC(x,y,0,0) >= d && norm_depth.atXYZC(x,y,0,0)< (d+1)) {
					for (int c = 0; c < input.spectrum(); c++) {
						result.atXYZC(x - norm_depth.atXYZC(x,y,0,0), y, 0, c) = input.atXYZC(x, y, 0, c);
					}
				}
			}

		}
	}
	// result.display("shifted image b4 handling blacks");
	// handling black gaps
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (result.atXYZC(x, y, 0, 0) == 0 && result.atXYZC(x, y, 0, 1) ==  0 && result.atXYZC(x, y, 0, 2) == 0) {
				for (int c = 0; c < input.spectrum(); c++) {
					result.atXYZC(x, y, 0, c) = input.atXYZC(x+1, y, 0, c);
				}
			}
		}

	}
	// result.display("shifted image after handling blacks");
	return result;
}

CImg<double> get_anaglyph(const CImg<double>& input, const CImg<double>& shifted_image)
{
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			result.atXYZC(x, y, 0, 1) = input.atXYZC(x, y, 0, 1); //green
			result.atXYZC(x, y, 0, 2) = input.atXYZC(x, y, 0, 2); //blue
			result.atXYZC(x, y, 0, 0) = shifted_image.atXYZC(x, y, 0, 0); //red from shifted image
		}

	}
	result.display("Look 3D!");
	return result;
}
CImg<double> depth_shift_image(const CImg<double>& input, const CImg<double>& depth, double normalization_param) {
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	CImg<double> norm_depth = nomalize_depth(depth, normalization_param);
	CImg<double> shifted_image = shifted_img(input, norm_depth, normalization_param);
	
	result = get_anaglyph(input, shifted_image);
	return result;
}

int main(int argc, char *argv[]) {
	if(argc < 3) {
		cerr << "usage: " << argv[0] << " image_file disp_file [normalization_param]" << endl;
		return 1;
	}

	string input_filename1 = argv[1], input_filename2 = argv[2];
	double normalization_param = 10; //default normalization parameter
	if (argc > 3) {
		normalization_param = std::atof(argv[3]);
	}

	CImg<double> image_rgb(input_filename1.c_str());
	CImg<double> image_disp(input_filename2.c_str());

	CImg<double> image_result = depth_shift_image(image_rgb, image_disp, normalization_param);
	image_result.normalize(0,255);
	image_result.save((input_filename1 + "-stereogram.png").c_str());
	
	return 0;
}
