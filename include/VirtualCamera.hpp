#pragma once

#include <string>
#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

using std::string;
using cv::Mat;

class VirtualCamera 
{
	private:
		Mat Perspective;// = Mat::eye(3, 3, CV_32F);
		double panangle;
		double tiltangle;
		double xtrans;
		double ytrans;
	public:
		VirtualCamera();
		Mat updateView(char key);
};
