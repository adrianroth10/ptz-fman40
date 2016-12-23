#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

#include "lilo.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

typedef float F;
typedef Vec<F, 3> VF;

Lilo::Lilo()
{
}

double Lilo::linear(int x, double a, double b)
{
	return a * x + b;
}

double Lilo::sigmoid(int x, double xshift)
{
	double a = -0.1;
	return 1.0 / (1 + exp(a * (x - xshift)));
}

vector<int> Lilo::findBox(Mat img)
{
	int minx = img.cols,
	    miny = img.rows,
	    maxx = 0,
	    maxy = 0;

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			VF point;
			point = img.at<VF>(y, x);
			if (point[0] != 0) {
				if (x < minx) {
					minx = x;
				}
				if (x > maxx) {
					maxx = x;
				}
				if (y < miny) {
					miny = y;
				}
				if (y > maxy) {
					maxy = y;
				}
			}
		}
	}
	vector<int> vec(4);
	vec[0] = minx;
	vec[1] = maxx;
	vec[2] = miny;
	vec[3] = maxy;
	return vec;
}

void Lilo::getBlendingMats(Mat &whiteOut2, Mat &white1, Mat H1, Mat H2)
{
	Mat white2, whiteOut1;
	white2 = white1.clone();
	whiteOut1 = whiteOut2.clone();

	warpPerspective(white1, whiteOut1, H1, whiteOut1.size());
	warpPerspective(white2, whiteOut2, H2, whiteOut2.size());

	Mat overlap;
	multiply(whiteOut1, whiteOut2, overlap, 1, CV_32F);

	vector<int> box;
	box = findBox(overlap);
	/*
	double a, b;
	a = -1.0 / (box[1] - box[0]);
	b = -a * box[1];
	*/
	double xshift;
	xshift = box[0] + (box[1] - box[0]) / 2;

	for (int y = box[2]; y <= box[3]; y++) {
		for (int x = box[0]; x <= box[1]; x++) {
			VF point;
			point = overlap.at<VF>(y, x);

			VF *point1, *point2;
			point1 = whiteOut1.ptr<VF>(y, x);
			point2 = whiteOut2.ptr<VF>(y, x);
			//cout << point << '\n';
			if (point[0] != 0) {
				double f1, f2;
				//f1 = linear(x, a, b);
				//f2 = 1 - f1;
				f2 = sigmoid(x, xshift);
				f1 = 1 - f2;
				//whiteOut1.at<float>(y, x) = f1;
				//whiteOut2.at<float>(y, x) = f2;
				*point1 *= f1;
				*point2 *= f2;
			}
		}
	}
	white1 = whiteOut1;
}

Mat Lilo::blend(Mat img1, Mat img2, Mat H, Mat PTZ, bool imAug)
{
	Mat H1 = PTZ * H;
	Mat H2 = PTZ;
	if (imAug == true) {
		H1 = Mat::eye(3, 3, CV_64F);
		H2 *= H;
	}

	Mat white1(img1.rows, img1.cols, CV_32FC3, VF(1, 1, 1));
	Mat white2(img1.rows, img1.cols, CV_32FC3);

	getBlendingMats(white2, white1, H1, H2);

	Mat out1(img1.rows, img1.cols, CV_8UC3);
	Mat out2(img1.rows, img1.cols, CV_8UC3);

	warpPerspective(img1, out1, H1, out1.size());
	warpPerspective(img2, out2, H2, out2.size());

	multiply(out1, white1, out1, 1, CV_8UC3);
	multiply(out2, white2, out2, 1, CV_8UC3);

	Mat outout(out1.rows, out1.cols, CV_8UC3);
	addWeighted(out1, 1, out2, 1, 0, outout, CV_8UC3);

	const string test1 = "Test1";
	namedWindow(test1, WINDOW_NORMAL);
	imshow( test1, white1 );
	const string test2 = "Test2";
	namedWindow(test2, WINDOW_NORMAL);
	imshow( test2, white2 );

	return outout;
}

Mat Lilo::calcHomography(Mat img1, Mat img2)
{
	//-- Step 1: Detect the keypoints using SURF Detector
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

	//printf("-- Max dist : %f \n", max_dist);
	//printf("-- Min dist : %f \n", min_dist);

	// Extracting the best matches
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++) {
		if (matches[i].distance < 3*min_dist) {
			good_matches.push_back(matches[i]);
		}
	}

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

	return H;
}

Mat Lilo::stitch(Mat img1, Mat img2)
{
	Mat H = calcHomography(img1, img2);

	Mat out= blend(img1, img2, H, Mat::eye(3, 3, CV_64F), 0);

	return out;
}
