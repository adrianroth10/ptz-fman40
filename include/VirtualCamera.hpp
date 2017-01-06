#pragma once

#include <string>
#include <opencv2/opencv.hpp>


#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

using std::string;
using cv::Mat;

class VirtualCamera 
{
	private:
		double panangle;
		double tiltangle;
		double tiltOffsetAngle;
		double angleInc;
		Mat tOM;
		Mat RotTilt;
		Mat RotPan;
	public:
		VirtualCamera(double tiltOffset=0.47179832679, double angleIncrement=M_PI/100.0);
		void updateView(char key, Mat& Rot, Mat& Zoom);
};
