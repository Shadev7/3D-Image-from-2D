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

CImg<int> get_mask(const CImg<int>& depth_img, int depth) {
	CImg<int> result(depth_img);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			result.atXYZC(x, y, 0, 0) = result.atXYZC(x, y, 0, 0) == depth;
		}
	}
	return result;
}

std::vector<int> get_unique_values(const CImg<int>& img) {
	std::vector<int> unique;

	for (int y = 0; y < img.height(); y++) {
		for (int x = 0; x < img.width(); x++) {
			int pixel = img.atXYZC(x, y, 0, 0);
			if (std::find(unique.begin(), unique.end(), pixel) == unique.end()) {
				unique.push_back(pixel);
			}
		}
	}
	return unique;
}

CImg<int> multiply(const CImg<int>& input, const CImg<int>& mask) {
	CImg<int> result(input);

	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (mask.atXYZC(x, y, 0, 0)) {
				continue;
			}
			for (int c = 0; c < result.spectrum(); c++) {
				result.atXYZC(x, y, 0, c) = 0;
			}
		}
	}
	return result;
}

CImg<int> translate_image(const CImg<int>& input, int pixels) {
	CImg<int> result(input.width(), input.height(), 1, 1, 0);

	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (x + pixels < input.width() && x+pixels >= 0) {
				for (int c = 0; c < input.spectrum(); c++) {
					result.atXYZC(x, y, 0, c) = input.atXYZC(x + pixels, y, 0, c);
				}
			}
		}
	}

	return result;
}

CImg<double> nomalize_depth(const CImg<double>& depth) {
	CImg<double> result(depth.width(), depth.height(), 1, 1, 0);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			result.atXYZC(x, y, 0, 0) = 30*(depth.atXYZC(x, y, 0, 0) - 127.5)/127.5;
		}
	}
	return result;
}
CImg<double> nomalize_depth2(const CImg<double>& depth) {
	CImg<double> result(depth.width(), depth.height(), 1, 1, 0);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			result.atXYZC(x, y, 0, 0) = -30*(depth.atXYZC(x, y, 0, 0) - 127.5)/127.5;
		}
	}
	return result;
}


CImg<double> shifted_img3(const CImg<double>& input, const CImg<double>& norm_depth)
{
	//CImg<double> result(input.width(), input.height(), 1, 1, 3);
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	result.assign(input);
	
	
	
	for (int d = -10; d<= 10; d++)
	{
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (norm_depth.atXYZC(x,y,0,0) >= d && norm_depth.atXYZC(x,y,0,0)< (d+1))
			{
				for (int c = 0; c < input.spectrum(); c++) {
					result.atXYZC(x, y, 0, c) = input.atXYZC(x - norm_depth.atXYZC(x,y,0,0), y, 0, c);
					//result.atXYZC(x + norm_depth.atXYZC(x,y,0,0), y, 0, c) = input.atXYZC(x, y, 0, c);
				}
			}
		}

	}
	}
}


CImg<double> shifted_img(const CImg<double>& input, const CImg<double>& norm_depth)
{
	//CImg<double> result(input.width(), input.height(), 1, 1, 3);
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	//result.assign(input);
	for (int d = -30; d<= 30; d++)
	{
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (norm_depth.atXYZC(x,y,0,0) >= d && norm_depth.atXYZC(x,y,0,0)< (d+1))
			{
				for (int c = 0; c < input.spectrum(); c++) {
					//result.atXYZC(x, y, 0, c) = input.atXYZC(x - norm_depth.atXYZC(x,y,0,0), y, 0, c);
					result.atXYZC(x + norm_depth.atXYZC(x,y,0,0), y, 0, c) = input.atXYZC(x, y, 0, c);
				}
			}
		}

	}
	}
	
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (result.atXYZC(x, y, 0, 0) == 0 && result.atXYZC(x, y, 0, 1) ==  0 && result.atXYZC(x, y, 0, 2) == 0)
			{
				for (int c = 0; c < input.spectrum(); c++) {
					//result.atXYZC(x, y, 0, c) = input.atXYZC(x + norm_depth.atXYZC(x,y,0,0), y, 0, c);
					//result.atXYZC(x, y, 0, c) = input.atXYZC(x, y, 0, c);
				}
			}
		}

	}
	//result.resize(input.width(), input.height(), 1, 3, 0);
	return result;
}
CImg<double> shifted_img2(const CImg<double>& input, const CImg<double>& norm_depth)
{
	//CImg<double> result(input.width(), input.height(), 1, 1, 3);
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	//result.assign(input);
	for (int d = -30; d<= 30; d++)
	{
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (norm_depth.atXYZC(x,y,0,0) >= d && norm_depth.atXYZC(x,y,0,0)< (d+1))
			{
				for (int c = 0; c < input.spectrum(); c++) {
					result.atXYZC(x, y, 0, c) = input.atXYZC(x - norm_depth.atXYZC(x,y,0,0), y, 0, c);
					//result.atXYZC(x - norm_depth.atXYZC(x,y,0,0), y, 0, c) = input.atXYZC(x, y, 0, c);
				}
			}
		}

	}
	}
	
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			if (result.atXYZC(x, y, 0, 0) == 0 && result.atXYZC(x, y, 0, 1) ==  0 && result.atXYZC(x, y, 0, 2) == 0)
			{
				for (int c = 0; c < input.spectrum(); c++) {
					//result.atXYZC(x, y, 0, c) = input.atXYZC(x + norm_depth.atXYZC(x,y,0,0), y, 0, c);
					//result.atXYZC(x, y, 0, c) = input.atXYZC(x, y, 0, c);
				}
			}
		}

	}
	return result;
}
CImg<double> get_anaglyph(const CImg<double>& input, const CImg<double>& shifted_image)
{
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	//result.assign(input);
	for (int y = 0; y < result.height(); y++) {
		for (int x = 0; x < result.width(); x++) {
			//result.atXYZC(x, y, 0, 0) = input.atXYZC(x, y, 0, 0);
			result.atXYZC(x, y, 0, 1) = input.atXYZC(x, y, 0, 1);
			result.atXYZC(x, y, 0, 2) = input.atXYZC(x, y, 0, 2);
			//if (shifted_image.atXYZC(x, y, 0, 1) !=  0 || shifted_image.atXYZC(x, y, 0, 2) != 0)
			{
			result.atXYZC(x, y, 0, 0) = shifted_image.atXYZC(x, y, 0, 0);
			//result.atXYZC(x, y, 0, 1) = shifted_image.atXYZC(x, y, 0, 1);
			//result.atXYZC(x, y, 0, 2) = shifted_image.atXYZC(x, y, 0, 2);
			}
		}

	}
	//result.display("temp!");
	return result;
}
CImg<double> depth_shift_image(const CImg<double>& input, const CImg<double>& depth, double factor) {
	//CImg<double> result(input.width(), input.height(), 1, 1, 3);
	CImg<double> result(input.width(), input.height(), 1, 3, 0);
	CImg<double> norm_depth = nomalize_depth(depth);
	CImg<double> norm_depth2 = nomalize_depth(depth);
	CImg<double> shifted_image = shifted_img(input, norm_depth);
	CImg<double> shifted_image2 = shifted_img2(input, norm_depth2);
	
	result = get_anaglyph(input, shifted_image);
	//result = get_anaglyph(shifted_image2, shifted_image);
	/*result.get_shared_channel(0) = 0.0;

	std::vector<int> unique_depths(get_unique_values(depth));
	std::sort(unique_depths.begin(), unique_depths.end());
	for (std::vector<int>::iterator it = unique_depths.begin()+1; it != unique_depths.end(); it++) {
		CImg<int> mask = get_mask(depth, *it);
		//mask.display("mask");
		CImg<int> layer = multiply(input, mask);
		//layer.display("layer");
		//std::cout<<"Translating depths at: "<<*it<<" by "<<*it * factor<<" pixels"<<std::endl;
		CImg<int> temp_img = translate_image(layer, *it * factor);
		result.get_shared_channel(0) += temp_img.get_channel(0);
	}*/
	shifted_image.display("Shift!");
	//result.display("Here!");
	return result;
}

int main(int argc, char *argv[]) {
	if(argc < 3) {
		cerr << "usage: " << argv[0] << " image_file disp_file [factor]" << endl;
		return 1;
	}

	string input_filename1 = argv[1], input_filename2 = argv[2];
	double factor = 0.1;
	if (argc > 3) {
		factor = std::atof(argv[3]);
	}

	CImg<double> image_rgb(input_filename1.c_str());
	CImg<double> image_disp(input_filename2.c_str());

	CImg<double> image_result = depth_shift_image(image_rgb, image_disp, factor);
	image_result.normalize(0,255);
	image_result.save((input_filename1 + "-stereogram.png").c_str());
	image_result.display("Look!");

	return 0;
}
