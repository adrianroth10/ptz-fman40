#pragma once

#include <string>
#include <opencv2/opencv.hpp>

using std::string;
using cv::Mat;

class Camera
{
	public:
		Camera(string file);
		Mat getCameraMatrix();
		Mat getDistortCoefficients();

	private:
		string file;
		Camera();
};
