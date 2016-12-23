#include <stdio.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "camera.hpp"
#include "lilo.hpp"
#include "VirtualCamera.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

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
	Mat newimg1, newimg2, newimg3;
	Mat img1_gray,img2_gray,img3_gray;

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
	remap(img3,
	      newimg3,
	      rectmap1,
	      rectmap2,
	      INTER_LINEAR);

	cvtColor(newimg1, img1_gray, COLOR_BGR2GRAY);
	cvtColor(newimg2, img2_gray, COLOR_BGR2GRAY);
	cvtColor(newimg3, img3_gray, COLOR_BGR2GRAY);
	img1 = newimg1;
	img2 = newimg2;
	img3 = newimg3;
	Lilo lilo;
	Mat H12 = lilo.calcHomography(img1, img2);
	Mat H23 = lilo.calcHomography(img3, img2);

	double xtrans=0.0;
	Mat trans=(Mat1d(3,3) << 1.0,0.0,xtrans,0.0,1.0,0.0,0.0,0.0,1.0);
//	Mat out1(img1.rows, img1.cols + xtrans, CV_8UC3);
//	Mat out2(img1.rows, img1.cols + xtrans, CV_8UC3);
//	Mat out3(img1.rows, img1.cols + xtrans, CV_8UC3);
//	Mat out4(img1.rows, img1.cols + xtrans, CV_8UC3);
	Mat TransZe = newCameraMatrix;

	
	
	char key;
	VirtualCamera Vcam=VirtualCamera();
	do{
		std::cout<<"Key pressed:"<<key<<std::endl;
		key=waitKey(0);
		Mat PTZ=TransZe*Vcam.updateView(key)*TransZe.inv();
		//Mat res12=trans*TransZe*PTZ*TransZe.inv()*H12;
		//Mat transPTZ=trans*TransZe*PTZ*TransZe.inv();
		
		Mat out1 = lilo.blend(img1, img2, H12, PTZ ,0);
		Mat outout = lilo.blend(out1 ,img3, H23, PTZ,1);

		const string test1 = "Test1";
		namedWindow(test1, WINDOW_NORMAL);
		imshow( test1, img3 );
		const string test2 = "Test2";
		namedWindow(test2, WINDOW_NORMAL);
		imshow( test2, out1 );
		const string test3 = "Test3";
		namedWindow(test3, WINDOW_NORMAL);
		imshow( test3, outout );	
		}while ((key != '\n') && (key != 27));
	std::cout<<"Exiting program"<<std::endl;
	//return 0;
}

