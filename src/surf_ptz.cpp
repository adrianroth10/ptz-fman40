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

	cameraMatrix = c1.getNewCameraMatrix();

	Mat H23 = Lilo::calcHomography(img2, img3);
	Mat H12 = Lilo::calcHomography(img2, img1);

	double xtrans=0.0;
	Mat trans=(Mat1d(3,3) << 1.0,0.0,xtrans,0.0,1.0,0.0,0.0,0.0,1.0);
	MatStruct imga1 = MatStruct(img1.size());
	Mat white1(img1.size(), CV_32FC3, Vec<float,3>(1, 1, 1));
	imga1.img=img1;
	MatStruct imga2 = MatStruct(img1.size());
	imga2.img=img2;
	MatStruct imga3=MatStruct(img1.size());
	imga3.img=img3;

	const string imgtit="image";
	const string masktit="mask";
	namedWindow(masktit,WINDOW_NORMAL);
	imshow(masktit,imga1.mask);


	char key;
	VirtualCamera Vcam=VirtualCamera();
	Mat Rot = Mat::eye(3, 3, CV_64F);
	Mat Zoom = Mat::eye(3, 3, CV_64F);
	Mat Trans = Mat::eye(3, 3, CV_64F);
	Mat eye = Mat::eye(3, 3, CV_64F);
	Mat PTZ;
	do{
		std::cout<<"Key pressed:"<<key<<std::endl;
		key=waitKey(0);
		Vcam.updateView(key, Rot, Zoom, Trans);
		PTZ=Trans * cameraMatrix * Zoom * Rot * cameraMatrix.inv();
		Mat H1 = PTZ * H12;
		Mat H2 = PTZ * H23;
		MatStruct out1 = Lilo::blend(imga1, imga2, H1, PTZ);
		MatStruct outout = Lilo::blend(out1 ,imga3, eye, H2);

		//const string test1 = "Test1";
		//namedWindow(test1, WINDOW_NORMAL);
		//imshow( test1, img3 );
		//const string test2 = "Test2";
		//namedWindow(test2, WINDOW_NORMAL);
		//imshow( test2, out1 );
		const string test3 = "Test3";
		namedWindow(test3, WINDOW_NORMAL);
		imshow( test3, outout.img );
		imshow( masktit, outout.mask);
		}while ((key != '\n') && (key != 27));
	std::cout<<"Exiting program"<<std::endl;
	//return 0;
}

