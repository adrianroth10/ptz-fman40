#pragma once

#include <string>
#include <opencv2/opencv.hpp>

using std::string;
using cv::Mat;

class VirtualCamera 
{
	private:
		Mat Perspective;// = Mat::eye(3, 3, CV_32F);

	public:
		VirtualCamera();
		Mat updateView(char key);
};