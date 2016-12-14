#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>

#include "VirtualCamera.hpp"
#include "camera.hpp"

using namespace cv;
using namespace std;


#ifdef LINUX
const string data_loc = "../data/";
#else
const string data_loc = "../../data/";
#endif

int main()
{
	String file = data_loc + "camera";
	Camera c(file);
	//Defining PI const
	const double PI = std::atan(1.0) * 4;


	// Rectification
	Mat cameraMatrix;
	Mat distortCoefficients;
	Mat newCameraMatrix;
	Mat rectmap1, rectmap2;
	Mat newimg1, newimg2, newimg3;
	Mat img1_gray, img2_gray, img3_gray;
	vector< Mat > vImg;
	Mat img1, img2, img3, rImg;

	img1 = imread(data_loc + "20150521_194353_49E3.jpg", 1);
	img2 = imread(data_loc + "20150521_194353_FD1E.jpg", 1);
	img3 = imread(data_loc + "20150521_194353_C1D8.jpg", 1);

	cameraMatrix = c.getCameraMatrix();
	distortCoefficients = c.getDistortCoefficients();

	newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix,
		distortCoefficients,
		img1.size(),
		1.0,
		img1.size());
	initUndistortRectifyMap(cameraMatrix,
		distortCoefficients,
		noArray(),
		newCameraMatrix,
		img1.size(),
		CV_32FC1,
		rectmap1,
		rectmap2);
	remap(img1,
		newimg1,
		rectmap1,
		rectmap2,
		INTER_LINEAR,
		BORDER_CONSTANT,
		Scalar(0, 0, 0, 0));
	remap(img2,
		newimg2,
		rectmap1,
		rectmap2,
		INTER_LINEAR,
		BORDER_CONSTANT,
		Scalar(0, 0, 0, 0));
	remap(img3,
		newimg3,
		rectmap1,
		rectmap2,
		INTER_LINEAR,
		BORDER_CONSTANT,
		Scalar(0, 0, 0, 0));

	cvtColor(newimg1, img1_gray, COLOR_BGR2GRAY);
	cvtColor(newimg2, img2_gray, COLOR_BGR2GRAY);
	cvtColor(newimg3, img3_gray, COLOR_BGR2GRAY);
	//img1 = newimg1;
	img2 = newimg2;
	img3 = newimg3;
	/*
	const string stitching_window = "Normal";
	namedWindow(stitching_window, WINDOW_NORMAL);
	imshow(stitching_window, img1);
	*/
	//while (waitKey(0) != 'w');
	double theta = PI/10.0;
	Mat out(newimg1.size().height + 1000, newimg1.size().width + 1000, 3);
	Mat outtemp = out;
	Mat inv;
	Mat Rot = (Mat1d(3, 3) << cos(theta), -sin(theta),1000.0, sin(theta), cos(theta),500.0,0.0,0.0,1.0);
	Mat Trans = ((Mat1d(3, 3) << 1.0, 0.0, 1000.0, 0.0, 1.0, 500.0, 0.0, 0.0, 1.0));
	//warpPerspective(newimg1, outtemp, Trans, out.size());

	Mat TransZe = (Mat1d(3, 3) << 1.0, 0.0, 940.400694, 0.0, 1.0, 539.433057, 0.0, 0.0, 1.0);
	Mat invTransZe = TransZe.inv();
	Mat Perspective = (Mat1d(3, 3) << 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	Mat PanTrans = (Mat1d(3, 3) << 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	VirtualCamera cam=VirtualCamera();

	int key = -1;
	do
	{
		cout << key << endl;
		//invert(cameraMatrix, inv,DECOMP_SVD);

		//cout << Perspective << endl;
		//cout << TransZe << endl;
		//cout << outtemp.size().height << endl;
		//cout << key << endl;
		/*
		if (key == 'w')
		{
			Perspective.at<double>(2, 1) -= 0.00001;
			Perspective.at<double>(1, 2) += 10;
		} else if (key == 's') {
			Perspective.at<double>(2, 1) += 0.00001;
			Perspective.at<double>(1, 2) -= 10;
		}
		else if (key == 'd') {
			Perspective.at<double>(2, 0) += 0.00005;
			Perspective.at<double>(0,2) -= 10;
		}
		else if (key == 'a') {
			Perspective.at<double>(2, 0) -= 0.00005;
			Perspective.at<double>(0,2) += 10;
		}
		else if (key == 'r') {
			Perspective.at<double>(0,0) += 0.01;
			Perspective.at<double>(1,1) += 0.01;
		}
		else if (key == 't') {
			Perspective.at<double>(0, 0) -= 0.01;
			Perspective.at<double>(1, 1) -= 0.01;
		}
		*/
		//cout << "Pan translation " <<PanTrans << endl;
		Perspective = cam.updateView(key);
		cout << "Perspective grid: " <<Perspective << endl;
		warpPerspective(newimg1, out, TransZe*Perspective*invTransZe, newimg1.size());

		Mat test;

		warpPerspective(img1, test, TransZe*Perspective*invTransZe, img1.size());
		/*
		const string Rot_window = "Rotated";
		namedWindow(Rot_window, WINDOW_NORMAL);
		imshow(Rot_window, test);
		*/

		const string Per_window = "Perspective";
		namedWindow(Per_window, WINDOW_NORMAL);
		imshow(Per_window, out);
		key = waitKey(0);
	} while (key != 27);

}
