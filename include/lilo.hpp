#pragma once

using cv::Mat;
using std::vector;

class Lilo
{
	public:
		Lilo();
		//Change to Mat vector of multiple images
		Mat blend(Mat img1, Mat img2, Mat H, Mat PTZ, bool imAug);
		Mat calcHomography(Mat img1, Mat img2);
		Mat stitch(Mat img1, Mat img2);
	private:
	// include attributes for fine tuning
		int minHessian = 400;
		double linear(int x, double a, double b);
		double sigmoid(int x, double xshift);
		vector<int> findBox(Mat img);
		void getBlendingMats(Mat &white1, Mat &white2, Mat H1, Mat H2);
};

