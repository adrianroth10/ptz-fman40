#include "VirtualCamera.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

using cv::Mat1d;


VirtualCamera::VirtualCamera() 
{
	Perspective = (Mat1d(3, 3) << 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	tiltangle=0.0;
	panangle=0.0;
}


Mat VirtualCamera::updateView(char key) 
{
	//std::cout<<M_PI<<std::endl;
	if (key == 'w')
	{
		//Perspective.at<double>(2, 1) -= 0.00001;
		Perspective.at<double>(1, 2) += 0.05;
		
		tiltangle-=M_PI/100.0;
	}
	else if (key == 's') {
		//Perspective.at<double>(2, 1) += 0.00001;
		Perspective.at<double>(1, 2) -= 0.05;
		tiltangle+=M_PI/100.0;
	}
	else if (key == 'd') {
		//Perspective.at<double>(2, 0) += 0.00005;
		Perspective.at<double>(0, 2) -= 0.05;
		panangle+=M_PI/100.0;
	}
	else if (key == 'a') {
		//Perspective.at<double>(2, 0) -= 0.00005;
		Perspective.at<double>(0, 2) += 0.05;
		panangle-=M_PI/100.0;
	}
	else if (key == 'r') {
		Perspective.at<double>(0, 0) += 0.01;
		Perspective.at<double>(1, 1) += 0.01;
	}
	else if (key == 't') {
		Perspective.at<double>(0, 0) -= 0.01;
		Perspective.at<double>(1, 1) -= 0.01;
	}
	
	Mat RotTilt = (Mat1d(3,3)<<1.0,0.0,0.0, 0.0,cos(tiltangle),-sin(tiltangle), 0.0,sin(tiltangle),cos(tiltangle));
	Mat RotPan = (Mat1d(3,3)<<cos(panangle),0.0,-sin(panangle), 0.0,1.0,0.0, sin(panangle),0.0,cos(panangle));
	std::cout<<RotTilt<<std::endl;
	std::cout<<RotPan<<std::endl;
	Mat H=Perspective*RotTilt*RotPan;
	std::cout<<Perspective<<std::endl;
	return H;//*RotPan*RotTilt;
}