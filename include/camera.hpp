#pragma once

class Camera
{
	public:
		Camera(std::string file);
		cv::Mat getCameraMatrix();
		cv::Mat getDistortCoefficients();

	private:
		std::string file;
		Camera();
};
