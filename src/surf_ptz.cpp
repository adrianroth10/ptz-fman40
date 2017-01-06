#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "Camera.hpp"
#include "Lilo.hpp"
#include "VirtualCamera.hpp"
#include "MatStruct.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

//struct imgAlpha{
//	Mat img;
//	Mat mask;
//};

int main( int argc, char** argv )
{
	Camera c1(-1);
	Camera c2(-2);
	Camera c3(-3);

	Mat img1, img2, img3;

	img1 = c1.click();
	img2 = c2.click();
	img3 = c3.click();
	img1 = c1.undistortRectifyImage(img1);
	img2 = c2.undistortRectifyImage(img2);
	img3 = c3.undistortRectifyImage(img3);

	// Rectification
	Mat cameraMatrix;
	Mat newimg1, newimg2, newimg3;

	//calibration matrices
	cameraMatrix = c1.getNewCameraMatrix();

	//Estimate Stitching homographies
	Mat H23 = Lilo::calcHomography(img2, img3);
	Mat H12 = Lilo::calcHomography(img2, img1);

	//Create structures for the alpha-channels
	MatStruct imga1 = MatStruct(img1.size());
	MatStruct imga2 = MatStruct(img1.size());
	MatStruct imga3 = MatStruct(img1.size());
	imga1.img=img1;
	imga2.img=img2;
	imga3.img=img3;

	const string imgtit="image";
	const string masktit="mask";
	namedWindow(imgtit,WINDOW_NORMAL);
	char key=-1;
	VirtualCamera Vcam;
	Mat Rot = Mat::eye(3, 3, CV_64F);
	Mat Zoom = Mat::eye(3, 3, CV_64F);
	Mat eye = Mat::eye(3, 3, CV_64F);
	Mat PTZ;
	do{
		Vcam.updateView(key, Rot, Zoom);
		PTZ=cameraMatrix * Zoom * Rot * cameraMatrix.inv();
		Mat H1 = PTZ * H12;
		Mat H2 = PTZ * H23;
		MatStruct out1 = Lilo::blend(imga1, imga2, H1, PTZ);
		MatStruct outout = Lilo::blend(out1 ,imga3, eye, H2);
		imshow( imgtit, outout.img );
		key=waitKey(0);
		std::cout<<"Key pressed:"<<key<<std::endl;
		}while ((key != '\n') && (key != 27));
	std::cout<<"Exiting program"<<std::endl;
}

