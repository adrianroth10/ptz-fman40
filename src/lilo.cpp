#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>

#include <string>

#include "lilo.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

Lilo::Lilo()
{
}

/*
double linear(int x, int zerox)
{

}
*/

Mat Lilo::blend(Mat img1, Mat img2, Mat H)
{
	double xtrans=2000.0;
	Mat trans=(Mat1d(3,3) << 1.0,0.0,xtrans,0.0,1.0,0.0,0.0,0.0,1.0);
	Mat res=trans*H;

	Mat out1(img1.rows, img1.cols + 2000, CV_8UC3);
	Mat out2(img1.rows, img1.cols + 2000, CV_8UC3);
	Mat out3(img1.rows, img1.cols + 2000, CV_8UC3);
	Mat out4(img1.rows, img1.cols + 2000, CV_8UC3);

	warpPerspective(img1, out1, res, out1.size());
	warpPerspective(img2, out2, trans, out2.size());

	Mat white1(img1.rows, img1.cols, CV_8UC3, Vec<uchar, 3>(1, 1, 1));
	Mat white2(img1.rows, img1.cols, CV_8UC3, Vec<uchar, 3>(1, 1, 1));

	Mat whiteOut1(out1.rows, out1.cols, 1);
	Mat whiteOut2(out1.rows, out1.cols, 1);

	warpPerspective(white1, whiteOut1, res, whiteOut1.size());
	warpPerspective(white2, whiteOut2, trans, whiteOut2.size());

	multiply(whiteOut1, whiteOut2, whiteOut1, 1, CV_8UC3);
	whiteOut2 = whiteOut1.clone();
	//multiply(whiteOut1, out1, out3, 1, CV_8UC3);
	//multiply(whiteOut1, out2, out4, 1, CV_8UC3);

	typedef uchar T;
	typedef Vec<T, 3> VT;
	int minx = whiteOut1.cols,
	    miny = whiteOut1.rows,
	    maxx = 0,
	    maxy = 0;

	cout << whiteOut1.size() << '\n';
	//return whiteOut1;

	for (int y = 0; y < whiteOut1.rows; y++) {
		for (int x = 0; x < whiteOut1.cols; x++) {
			VT point;
			point = whiteOut1.at<VT>(y, x);
			if (point[0] == 1 && point[1] == 1 && point[2] == 1) {
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

	double a1, a2, b1, b2;
	a1 = -1.0 / (maxx - minx);
	a2 = -a1;
	b1 = -a1 * maxx;
	b2 = -a2 * minx;

	for (int y = miny; y < maxy; y++) {
		for (int x = minx; x < maxx; x++) {
			VT point;
			point = whiteOut1.at<VT>(y, x);
			//cout << point << '\n';
			//cout << x << " " << y << '\n';

			VT *point1, *point2;
			point1 = out1.ptr<VT>(y, x);
			point2 = out2.ptr<VT>(y, x);
			//if (point[0] == 1) {
				double f1, f2;
				f1 = a1 * x + b1;
				f2 = a2 * x + b2;
				//cout << f1 << '\n';
				//cout << f2 << '\n';
				//cout << *point1 << '\n';
				//cout << *point2 << '\n';
				*point1 *= f1;
				*point2 *= f2;
				//*point1 += VT(f1 * 255, f1 * 255, f1 * 255);
				//*point2 += VT(f2 * 255, f2 * 255, f2 * 255);
				//cout << *point1 << '\n';
				//cout << *point2 << '\n';
			//}
		}
	}

	Mat outout(out1.rows, out1.cols, CV_8UC3);
	//addWeighted(out1, 1, out3, -0.5, 0, out1, CV_8UC3);
	//addWeighted(out2, 1, out4, -0.5, 0, out2, CV_8UC3);
	addWeighted(out1, 1, out2, 1, 0, outout, CV_8UC3);

	const string test1 = "Test1";
	namedWindow(test1, WINDOW_NORMAL);
	imshow( test1, out1 );
	const string test2 = "Test2";
	namedWindow(test2, WINDOW_NORMAL);
	imshow( test2, out2 );

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

	Mat out= blend(img1, img2, H);

	return out;
}
