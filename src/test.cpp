#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat img1,img2, img1_gray,img2_gray;
int thresh = 200;
int max_thresh = 255;

const string data_loc = "../../data/";
const string source_window = "Source image";
const string corners_window = "Corners detected";
const string feature_window = "FAST features";

/// Function header
void cornerHarris_demo( int, void* );

/** function main */
int main( int argc, char** argv )
{
	/// Load source image and convert it to gray
	//img = imread( argv[1], 1 );
	//cvtColor(img, img_gray, COLOR_BGR2GRAY);

	//Load in start images and covnert them to grayscale
	img1 = imread(data_loc+"20150521_194353_49E3.jpg", 1);
	img2 = imread(data_loc+"20150521_194353_C1D8.jpg", 1);
	cvtColor( img1, img1_gray, COLOR_BGR2GRAY );
	cvtColor(img2, img2_gray, COLOR_BGR2GRAY);

	/// Create a window and a trackbar
	//namedWindow( source_window, WINDOW_NORMAL );

	const string new_data = "second window";
	namedWindow(new_data, WINDOW_NORMAL);
	//createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
	//imshow( source_window, img1 );
	//imshow(new_data, img2);

	//find feature-points with FAST algorithm
	vector<KeyPoint> keypointsim1, keypointsim2;
	int FASTthreshold=100;
	bool nonMaxSuppression=1;
	Mat dst1, dst2;
	Mat descriptor1, descriptor2;

	FAST(img1_gray, keypointsim1, FASTthreshold, nonMaxSuppression);
	FAST(img2_gray, keypointsim2, FASTthreshold, nonMaxSuppression);


	//Extract features with ORB, from the FAST featruepoints.
	Ptr<DescriptorExtractor> orbDescriptor=ORB::create();
	int minHessian = 500;
	orbDescriptor->compute(img1_gray, keypointsim1, descriptor1);
	orbDescriptor->compute(img2_gray, keypointsim2, descriptor2);
	
	drawKeypoints(img1, keypointsim1, dst1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img1, keypointsim1, dst2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	namedWindow(feature_window, WINDOW_NORMAL);
	imshow(feature_window, dst1);
	imshow(new_data, dst2);

	//Match points.
	vector<DMatch> correspondences;
	BFMatcher Matcher = BFMatcher(NORM_L2, false);
	Matcher.match(descriptor1,descriptor2,correspondences);

	const string corr_win="Correspondences";
	namedWindow(corr_win, WINDOW_NORMAL);
	Mat output, mask;
	Scalar color = Scalar(255, 0, 0);
	drawMatches(img1, keypointsim1, img2, keypointsim2, correspondences, output, color, color, mask, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow(corr_win,output);

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
