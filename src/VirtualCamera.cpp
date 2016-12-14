#include "VirtualCamera.hpp"

using cv::Mat1d;


VirtualCamera::VirtualCamera() 
{
	Perspective = (Mat1d(3, 3) << 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}


Mat VirtualCamera::updateView(char key) 
{
	if (key == 'w')
	{
		Perspective.at<double>(2, 1) -= 0.00001;
		Perspective.at<double>(1, 2) += 10;
	}
	else if (key == 's') {
		Perspective.at<double>(2, 1) += 0.00001;
		Perspective.at<double>(1, 2) -= 10;
	}
	else if (key == 'd') {
		Perspective.at<double>(2, 0) += 0.00005;
		Perspective.at<double>(0, 2) -= 10;
	}
	else if (key == 'a') {
		Perspective.at<double>(2, 0) -= 0.00005;
		Perspective.at<double>(0, 2) += 10;
	}
	else if (key == 'r') {
		Perspective.at<double>(0, 0) += 0.01;
		Perspective.at<double>(1, 1) += 0.01;
	}
	else if (key == 't') {
		Perspective.at<double>(0, 0) -= 0.01;
		Perspective.at<double>(1, 1) -= 0.01;
	}
	return Perspective;
}
