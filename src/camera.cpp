#include "camera.hpp"

using cv::Mat1d;

Camera::Camera(string file)
{
	this->file = file;
}

Mat Camera::getCameraMatrix()
{
	double fx = 1645.29859;
	double fy = 1642.9788;
	double cx = 940.400694;
	double cy = 539.433057;

	Mat cameraMatrix = (Mat1d(3, 3) << fx, 0.0, cx, 0.0, fy, cy, 0.0, 0.0, 1.0);

	return cameraMatrix;
}

Mat Camera::getDistortCoefficients()
{
	Mat distortCoefficients = (Mat1d(1, 8) << -0.39205638, 0.3269371, 0.0, 0.0, -0.23, 0.0, 0.0, 0.0);

	return distortCoefficients;
}
