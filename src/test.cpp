#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "camera.hpp"

using namespace cv;
using namespace std;

// Different locations of the data
#ifdef LINUX
const string data_loc = "../data/";
#else
const string data_loc = "../../data/";
#endif
//////////////////////////////////

int main(int argc, char** argv)
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

	//FAST feature points
	vector<KeyPoint> keypointsim1, keypointsim2;
	int FASTthreshold;
	bool nonMaxSuppression;

	// ORB feature extraction
	Ptr<DescriptorExtractor> orbDescriptor;
	Mat descriptor1, descriptor2;
	Mat dst1, dst2;

	// Matching points
	vector<DMatch> correspondences;
	BFMatcher matcher;

	Scalar color;
	Mat output, mask;
	///////////////////////////////////////////////////



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

	cvtColor(newimg1, img1_gray, COLOR_BGR2GRAY);
	cvtColor(newimg2, img2_gray, COLOR_BGR2GRAY);
	img1 = newimg1;
	img2 = newimg2;

	// Find feature-points with FAST algorithm
	FASTthreshold = 130;
	nonMaxSuppression = 1;
	FAST(img1_gray, keypointsim1, FASTthreshold, nonMaxSuppression);
	FAST(img2_gray, keypointsim2, FASTthreshold, nonMaxSuppression);


	// Extract features with ORB, from the FAST featruepoints.
	orbDescriptor = ORB::create(500, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 51, 20);
	orbDescriptor->compute(img1_gray, keypointsim1, descriptor1);
	orbDescriptor->compute(img2_gray, keypointsim2, descriptor2);

	drawKeypoints(img1,
		      keypointsim1,
		      dst1,
		      Scalar::all(-1),
		      DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img2,
		      keypointsim2,
		      dst2,
		      Scalar::all(-1),
		      DrawMatchesFlags::DEFAULT);

	// Match points.
	color = Scalar(255, 0, 0);
	matcher = BFMatcher(NORM_L2, false);
	matcher.match(descriptor1,descriptor2,correspondences);
	drawMatches(img1,
		    keypointsim1,
		    img2,
		    keypointsim2,
		    correspondences,
		    output,
		    color,
		    color,
		    mask,
		    DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show images
	const string features1 = "First features";
	namedWindow(features1, WINDOW_NORMAL);
	imshow(features1, dst1);

	const string features2 = "Second features";
	namedWindow(features2, WINDOW_NORMAL);
	imshow(features2, dst2);

	const string corr_win = "Correspondences";
	namedWindow(corr_win, WINDOW_NORMAL);
	imshow(corr_win,output);

	//TODO:: Find Homography and stich images

	while (waitKey(0) != '\n');
	return(0);
}
