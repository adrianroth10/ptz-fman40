#include <stdio.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <string>

#include "camera.hpp"
#include "lilo.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

// Different locations of the data
#ifdef LINUX
const string data_loc = "../data/";
#else
const string data_loc = "../../data/";
#endif
//////////////////////////////////

int main( int argc, char** argv )
{
	String file = data_loc + "camera";
	Camera c(file);

	//////////// Declaring variables //////////////////
	Mat img1, img2, img3;

	// Rectification
	Mat cameraMatrix;
	Mat distortCoefficients;
	Mat newCameraMatrix;
	Mat rectmap1,rectmap2;
	Mat newimg1, newimg2;
	Mat img1_gray,img2_gray;

	// Load in start images and convert them to grayscale
	img1 = imread(data_loc+"20150521_194353_C1D8.jpg", 1);
	img2 = imread(data_loc+"20150521_194353_FD1E.jpg", 1);
	img3 = imread(data_loc+"20150521_194353_49E3.jpg", 1);

	// Rectification in two steps to show image distortion better
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
			        cameraMatrix,
			        img1.size(),
			        CV_32FC1,
			        rectmap1,
			        rectmap2);
	remap(img1,
	      newimg1,
	      rectmap1,
	      rectmap2,
	      INTER_LINEAR);
	remap(img2,
	      newimg2,
	      rectmap1,
	      rectmap2,
	      INTER_LINEAR);

	cvtColor(newimg1, img1_gray, COLOR_BGR2GRAY);
	cvtColor(newimg2, img2_gray, COLOR_BGR2GRAY);
	img1 = newimg1;
	img2 = newimg2;

	Lilo lilo;
	Mat outout = lilo.stitch(img1, img2);

	const string test3 = "Test3";
	namedWindow(test3, WINDOW_NORMAL);
	imshow( test3, outout );

	while (waitKey(0) != '\n');

	return 0;
}
