// Skeleton code for B657 A4 Part 1.
// D. Crandall
//
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <CImg.h>
#include <assert.h>

using namespace cimg_library;
using namespace std;

CImg<double> getDiparityImg (CImg<double> image_rgb,CImg<double> image_disp)
{
	int wid = image_rgb.width();
	int ht = image_rgb.height();
	double disparity_factor = 0.1;
	CImg<double> diparityImg(wid, ht, 1, 3, 0);
	//CImg<double> diparityImg = image_rgb;
	int px_shift;
	//cout << wid<< endl;
	//cout << ht<< endl;
	for (int i=0; i< wid ; i++)
	{
		for (int j=0; j< ht ; j++)
		{
			//cout << "1";
			px_shift = (int)(round(image_disp.atXY(i,j)*disparity_factor));
			//cout << image_disp.atXY(i,j);
			//cout << px_shift << endl;
		/* 	if (px_shift > 0)
			{
				cout << px_shift << endl;
			} */
			/* if (i+px_shift < 255)
			{
				//cout << i << "," <<j << endl;
				//diparityImg.atXY(i,j) = 0;
			} */
			//else
			//if (px_shift > 15 )	
			{
				//cout << "2";
				//diparityImg.atXY(i,j) = image_rgb.atXY(i,(j - px_shift));
				/* diparityImg.atXYZC(i, j, 0, 0) = image_rgb.atXYZC(i,(j - px_shift), 0, 0);
                diparityImg.atXYZC(i, j, 0, 1) = image_rgb.atXYZC(i,(j - px_shift), 0, 1);
                diparityImg.atXYZC(i, j, 0, 2) = image_rgb.atXYZC(i,(j - px_shift), 0, 2); */
				/* diparityImg.atXYZC(i, j, 0, 0) = image_rgb.atXYZC((i + px_shift),j, 0, 0);
                diparityImg.atXYZC(i, j, 0, 1) = image_rgb.atXYZC((i + px_shift),j, 0, 1);
                diparityImg.atXYZC(i, j, 0, 2) = image_rgb.atXYZC((i + px_shift),j, 0, 2); */
				diparityImg.atXYZC((i + px_shift), j, 0, 0) = image_rgb.atXYZC(i,j, 0, 0);
                diparityImg.atXYZC((i + px_shift), j, 0, 1) = image_rgb.atXYZC(i,j, 0, 1);
                diparityImg.atXYZC((i + px_shift), j, 0, 2) = image_rgb.atXYZC(i,j, 0, 2);
			}
			
		}
		//cout << endl;
	}
	return diparityImg;
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
