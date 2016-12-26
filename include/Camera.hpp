#pragma once

using std::string;
using cv::Mat;

class Camera
{
	public:
		Camera(int nbr);

		Mat click();

		Mat getNewCameraMatrix();

		Mat undistortRectifyImage(Mat input);

	private:
		// Attributes
		Mat newCameraMatrix;
		Mat rectmap1;
		Mat rectmap2;
		string camera_file;
		string source;

		// Internal methods
		Camera();

		void readFile(Mat &cameraMatrix,
			      Mat &distortCoefficients);
};
