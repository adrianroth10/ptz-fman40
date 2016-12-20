#pragma once

using cv::Mat;

class Lilo
{
	public:
		Lilo();
		//Change to Mat vector of multiple images
		Mat stitch(Mat img1, Mat img2);
	private:
	// include attributes for fine tuning
		int minHessian = 400;
		Mat calcHomography(Mat img1, Mat img2);
		Mat blend(Mat img1, Mat img2, Mat h);
};

