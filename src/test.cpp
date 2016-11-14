#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat img, img_gray;
int thresh = 200;
int max_thresh = 255;

const string data_loc = "../data/";
const string source_window = "Source image";
const string corners_window = "Corners detected";
const string feature_window = "FAST features";

/// Function header
void cornerHarris_demo( int, void* );

/** function main */
int main( int argc, char** argv )
{
	/// Load source image and convert it to gray
	img = imread( argv[1], 1 );
	cvtColor( img, img_gray, COLOR_BGR2GRAY );

	/// Create a window and a trackbar
	namedWindow( source_window, WINDOW_NORMAL );
	//createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
	imshow( source_window, img );

	//find features with FAST algorithm
	vector<KeyPoint> keypoints;
	int threshold=9;
	bool nonMaxSuppression=1;
	Mat dst;

	FAST(img_gray, keypoints, threshold, nonMaxSuppression);
	drawKeypoints(img_gray, keypoints, dst, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	namedWindow(feature_window, WINDOW_NORMAL);
	imshow(feature_window, dst);

	//cornerHarris_demo( 0, 0 );

	while (waitKey(0) != '\n');
	return(0);
}

/** @function cornerHarris_demo */
/*
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
*/
