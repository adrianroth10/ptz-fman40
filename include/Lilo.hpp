#pragma once

using cv::Mat;
using cv::Size;
using std::vector;
#include "MatStruct.hpp"

//#define LINEAR 0
//#define SIGMOID 1
enum BLENDTYPES {
	LINEAR,
	SIGMOID
};

class Lilo
{
	public:
		// Attribute
		static int minHessian;
		static int homographyThreshold;

		static MatStruct blend(MatStruct &img1,
				       MatStruct &img2,
				       Mat H1,
				       Mat H2,
				       Size s = Size(0, 0),
				       int blendType = SIGMOID);

		static Mat calcHomography(Mat &img1, Mat &img2);

		static Mat stitch(Mat &img1,
				  Mat &img2,
				  Size s = Size(0, 0),
				  int blendType = SIGMOID);

		static Mat stitch(Mat &img1,
				  Mat &img2,
				  Mat &img3,
				  Size s = Size(0, 0));

		static Mat stitch(Mat &img1,
				  Mat &img2,
				  Mat &img3,
				  Mat &img4,
				  Size s = Size(0, 0));
	private:
		// Internal functions
		static vector<int> findBox(Mat &img);

		static void getBlendingMats(Mat &whiteOut1,
					    Mat &whiteOut2,
					    Mat &white1,
					    Mat &white2,
					    Mat H1,
					    Mat H2,
					    int blendType);

		static double linear(int x, vector<int> box);

		static double sigmoid(int x, vector<int> box);
};

