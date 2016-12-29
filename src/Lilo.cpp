#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "Lilo.hpp"
#include "MatStruct.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

typedef float F;
typedef Vec<F, 3> VF;

int Lilo::minHessian = 400;
int Lilo::homographyThreshold = 3;

MatStruct Lilo::blend(MatStruct &img1, MatStruct &img2, Mat H1, Mat H2, Size s)
{
	if (s.width <= 0 || s.height <= 0) {
		s = img1.img.size();
	}

	Mat white1=img1.mask;//(img1.size(), CV_32FC3, VF(1, 1, 1));
	Mat white2=img2.mask;//(img2.size(), CV_32FC3, VF(1, 1, 1));
	Mat whiteOut1(s, CV_32FC3);
	Mat whiteOut2(s, CV_32FC3);

	getBlendingMats(whiteOut1, whiteOut2, white1, white2, H1, H2);

	Mat out1(s, CV_8UC3);
	Mat out2(s, CV_8UC3);

	warpPerspective(img1.img, out1, H1, out1.size());
	warpPerspective(img2.img, out2, H2, out2.size());

	multiply(out1, whiteOut1, out1, 1, CV_8UC3);
	multiply(out2, whiteOut2, out2, 1, CV_8UC3);

	Mat outout(out1.rows, out1.cols, CV_8UC3);
	addWeighted(out1, 1, out2, 1, 0, outout, CV_8UC3);
	MatStruct outa=MatStruct(outout.size());
	outa.img=outout;
	Mat mask=whiteOut1+whiteOut2;
	min(mask,1.0,mask);
	outa.mask=mask;

	return outa;
}

// calculates the homography from img2 to img1
Mat Lilo::calcHomography(Mat &img1, Mat &img2)
{
	// Step 1: Detect the keypoints using SURF Detector
	Ptr<SURF> detector = xfeatures2d::SURF::create(minHessian);

	std::vector<KeyPoint> keypoints_img1, keypoints_img2;

	detector->detect(img1, keypoints_img1);
	detector->detect(img2, keypoints_img2);

	// Step 2: Calculate descriptors (feature vectors)

	Mat descriptors_img1, descriptors_img2;

	detector->compute(img1, keypoints_img1, descriptors_img1);
	detector->compute(img2, keypoints_img2, descriptors_img2);

	// Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_img2, descriptors_img1, matches);

	double max_dist = 0;
	double min_dist = 100;

	// Quick calculation of max and min
	// distances between keypoints
	for (int i = 0; i < descriptors_img2.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	// Extracting the best matches
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_img2.rows; i++) {
		if (matches[i].distance < 3*min_dist) {
			good_matches.push_back(matches[i]);
		}
	}

	//-- Localize the object from img1 in img2
	std::vector<Point2f> img1_points;
	std::vector<Point2f> img2_points;

	for ( size_t i = 0; i < good_matches.size(); i++ )
	{
		// Get the keypoints from the good matches
		img2_points.push_back( keypoints_img2[ good_matches[i].queryIdx ].pt );
		img1_points.push_back( keypoints_img1[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography(img2_points, img1_points, CV_RANSAC, homographyThreshold);
	return H;
}

vector<int> Lilo::findBox(Mat &img)
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

void Lilo::getBlendingMats(Mat &whiteOut1,
			   Mat &whiteOut2,
			   Mat &white1,
			   Mat &white2,
			   Mat H1,
			   Mat H2)
{
	warpPerspective(white1, whiteOut1, H1, whiteOut1.size());
	warpPerspective(white2, whiteOut2, H2, whiteOut2.size());

	Mat overlap;
	multiply(whiteOut1, whiteOut2, overlap, 1, CV_32F);

	vector<int> box;
	box = findBox(overlap);

	double xshift;
	xshift = box[0] + (box[1] - box[0]) / 2;

	double f1, f2;
	for (int y = box[2]; y <= box[3]; y++) {
		for (int x = box[0]; x <= box[1]; x++) {
			VF point;
			point = overlap.at<VF>(y, x);

			if (point[0] != 0 &&
			    point[1] != 0 &&
			    point[2] != 0) {
				f1 = sigmoid(x, xshift);
				f2 = 1 - f1;
				whiteOut1.at<VF>(y, x) *= f1;;
				whiteOut2.at<VF>(y, x) *= f2;;
			}
		}
	}
}

double Lilo::sigmoid(int x, double xshift)
{
	double a = -0.1;
	return 1 - 1.0 / (1 + exp(a * (x - xshift)));
}

Mat Lilo::stitch(Mat &img1, Mat &img2, Size s)
{
	if (s.width <= 0 || s.height <= 0) {
		s = img1.size();
	}

	Mat img1_gray, img2_gray;
	cvtColor(img1, img1_gray, COLOR_BGR2GRAY);
	cvtColor(img2, img2_gray, COLOR_BGR2GRAY);

	Mat H1, H2, out;
	H1 = Mat::eye(3, 3, CV_64F);
	H2 = calcHomography(img1_gray, img2_gray);
	//Mat white1=(img1.size(), CV_32FC3, Vec<float,3>(1.0, 1.0, 1.0));
	MatStruct imga1=MatStruct(img1.size());
	MatStruct imga2=MatStruct(img2.size());
	imga1.img=img1;
	imga2.img=img2;
	MatStruct outa=MatStruct(img2.size());	

	if (H2.at<double>(0, 2) < 0) {
		H1.at<double>(0, 2) = s.width - img1.cols;
		H2 = H1 * H2;
		outa = blend(imga2, imga1, H2, H1, s);
	} else {
		outa = blend(imga1, imga2, H1, H2, s);
	}


	return outa.img;
}

Mat Lilo::stitch(Mat &img1, Mat &img2, Mat &img3, Size s)
{
	Size s2(s);
	s2.width = img1.cols + (s.width - img1.cols) / 2;

	Mat out;
	out = stitch(img1, img2, s2);
	out = stitch(out, img3, s);

	return out;
}

Mat Lilo::stitch(Mat &img1, Mat &img2, Mat &img3, Mat &img4, Size s)
{
	Size s2(s);
	s2.width = img1.cols + (s.width - img1.cols) * 2 / 3;

	Mat out;
	out = stitch(img1, img2, img3, s2);
	out = stitch(out, img4, s);

	return out;
}
