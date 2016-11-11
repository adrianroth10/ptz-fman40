#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat img, img_gray;
int thresh = 200;
int max_thresh = 255;

const string source_window = "Source image";
const string corners_window = "Corners detected";

/// Function header
void cornerHarris_demo( int, void* );

/** @function main */
int main( int argc, char** argv )
{
	/// Load source image and convert it to gray
	img = imread( argv[1], 1 );
	cvtColor( img, img_gray, COLOR_BGR2GRAY );

	/// Create a window and a trackbar
	namedWindow( source_window, WINDOW_NORMAL );
	createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
	imshow( source_window, img );

	cornerHarris_demo( 0, 0 );

	while (waitKey(0) != '\n');
	return(0);
}

/** @function cornerHarris_demo */
void cornerHarris_demo( int, void* )
{

	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros( img.size(), CV_32FC1 );

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarris( img_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

	/// Normalizing
	normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs( dst_norm, dst_norm_scaled );

	/// Drawing a circle around corners
	for( int j = 0; j < dst_norm.rows ; j++ )
	{ for( int i = 0; i < dst_norm.cols; i++ )
		{
			if( (int) dst_norm.at<float>(j,i) > thresh )
			{
				circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
			}
		}
	}
	/// Showing the result
	namedWindow( corners_window, WINDOW_NORMAL );
	imshow( corners_window, dst_norm_scaled );
}

/*
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int, char**) {
cv::VideoCapture vcap(0); // open standard camera
cv::Mat image;

const std::string videoStreamAddress = "http://<root:ptz-fman40>@<31.208.80.26:80>";

//open the video stream and make sure it's opened
if(!vcap.open(videoStreamAddress)) {
std::cout << "Error opening video stream or file" << std::endl;
return -1;
}
if(!vcap.isOpened())  // check if we succeeded
return -1;

for(;;) {
if(!vcap.read(image)) {
std::cout << "No frame" << std::endl;
cv::waitKey();
}
cv::imshow("Output Window", image);
if(cv::waitKey(1) >= 0) break;
}
}
*/

