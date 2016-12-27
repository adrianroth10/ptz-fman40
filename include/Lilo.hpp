#pragma once

using cv::Mat;
using cv::Size;
using std::vector;
#include "imga.hpp"

class Lilo
{
	public:
		// Attribute
		static int minHessian;
		static int homographyThreshold;

		static imga blend(imga &img1,
				 imga &img2,
				 Mat H1,
				 Mat H2,
				 Size s = Size(0, 0));

		static Mat calcHomography(Mat &img1, Mat &img2);
/*
		static Mat stitch(Mat &img1,
				  Mat &img2,
				  Size s = Size(0, 0));

		static Mat stitch(Mat &img1,
				  Mat &img2,
				  Mat &img3,
				  Size s = Size(0, 0));

		static Mat stitch(Mat &img1,
				  Mat &img2,
				  Mat &img3,
				  Mat &img4,
				  Size s = Size(0, 0));
*/	private:
		// Internal functions
		static vector<int> findBox(Mat &img);

		static void getBlendingMats(Mat &whiteOut1,
					    Mat &whiteOut2,
					    Mat &white1,
					    Mat &white2,
					    Mat H1,
					    Mat H2);

		static double sigmoid(int x, double xshift);
};

