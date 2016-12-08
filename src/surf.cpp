#include <stdio.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "camera.hpp"

using namespace cv;
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

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	Ptr<SURF> detector = xfeatures2d::SURF::create(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector->detect(img1, keypoints_object);
	detector->detect(img2, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)

	Mat descriptors_object, descriptors_scene;

	detector->compute(img1, keypoints_object, descriptors_object);
	detector->compute(img2, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++) {
		if (matches[i].distance < 3*min_dist) {
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(img1, keypoints_object, img2, keypoints_scene,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object from img_1 in img_2
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for ( size_t i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography( obj, scene, CV_RANSAC );
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0,0);
	obj_corners[1] = Point( img1.cols, 0 );
	obj_corners[2] = Point( img1.cols, img1.rows );
	obj_corners[3] = Point( 0, img1.rows );
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform( obj_corners, scene_corners, H);


	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	Point2f offset( (float)img1.cols, 0);
	line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 255, 0), 4);

	//-- Show detected matches
	//const string features = "Good Matches & Object detection";
	//namedWindow(features, WINDOW_NORMAL);
	//imshow( features, img_matches );

	double xtrans=2000.0;
	Mat trans=(Mat1d(3,3) << 1.0,0.0,xtrans,0.0,1.0,0.0,0.0,0.0,1.0);
	Mat res=trans*H;
	Mat out1(img1.size().height, img1.size().width + 2000, 3);
	Mat out2(img1.size().height, img1.size().width + 2000, 3);
	warpPerspective(img1, out1, res, out1.size());
	warpPerspective(img2, out2, trans, out2.size());

	Mat white1(img1.size().height, img1.size().width, CV_8U, (uchar)255);
	Mat white2(img1.size().height, img1.size().width, CV_8U, (uchar)255);

	Mat whiteOut1(out1.size().height, out1.size().width, 1);
	Mat whiteOut2(out1.size().height, out1.size().width, 1);

	warpPerspective(white1, whiteOut1, res, whiteOut1.size());
	warpPerspective(white2, whiteOut2, trans, whiteOut2.size());

	Mat outout(out1.rows, out1.cols, 3);
	for (int i = 0; i < out1.rows; i++) {
		for (int j = 0; i < out1.cols; j++) {
			if (whiteOut1.at(1, 1) == 255 && whiteOut2.at(1, 1) == 255) {
				outout.at(1, 1, 1) = (out1.at(1, 1, 1) + out2.at(1, 1, 1)) / 2;
			}
		}
	}

	const string test1 = "Test1";
	namedWindow(test1, WINDOW_NORMAL);
	imshow( test1, whiteOut1 );
	const string test2 = "Test2";
	namedWindow(test2, WINDOW_NORMAL);
	imshow( test2, whiteOut2 );

	while (waitKey(0) != '\n');

	return 0;
}
