#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>

#include "Camera.hpp"

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

	// Rectification
	Mat cameraMatrix;
	Mat distortCoefficients;
	Mat newCameraMatrix;
	Mat rectmap1,rectmap2;
	Mat newimg1, newimg2, newimg3;
	Mat img1_gray,img2_gray, img3_gray;
	vector< Mat > vImg;
	Mat img1, img2, img3, rImg;

	img1 = imread(data_loc+"20150521_194353_49E3.jpg", 1);
	img2 = imread(data_loc+"20150521_194353_FD1E.jpg", 1);
	img3 = imread(data_loc+"20150521_194353_C1D8.jpg", 1);

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
	img1 = newimg1;
	img2 = newimg2;
	img3 = newimg3;

	vImg.push_back(img1);
	vImg.push_back(img2);
	vImg.push_back(img3);

	Stitcher stitcher = Stitcher::createDefault();

	unsigned long AAtime=0, BBtime=0; //check processing time
	AAtime = getTickCount(); //check processing time

	Stitcher::Status status = stitcher.stitch(vImg, rImg);

	BBtime = getTickCount(); //check processing time
	printf("%.2lf sec \n",  (BBtime - AAtime)/getTickFrequency() ); //check processing time

	if (Stitcher::OK == status) {
		const string stitching_window = "Stitching";
		namedWindow(stitching_window, WINDOW_NORMAL);
		imshow(stitching_window,rImg);
	} else
		printf("Stitching fail.");

	while (waitKey(0) != '\n');
}
