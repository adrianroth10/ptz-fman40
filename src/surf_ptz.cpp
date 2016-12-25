#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "Camera.hpp"
#include "Lilo.hpp"
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
	string camera_file = data_loc + "camera_football.xml";
	string source1 = data_loc + "20150521_194353_C1D8.jpg";
	string source2 = data_loc + "20150521_194353_FD1E.jpg";
	string source3 = data_loc+"20150521_194353_49E3.jpg";
	Camera c1(source1, camera_file);
	Camera c2(source2, camera_file);
	Camera c3(source3, camera_file);

	Mat img1, img2, img3;

	img1 = c1.click();
	img2 = c2.click();
	img3 = c3.click();
	img1 = c1.undistortRectifyImage(img1);
	img2 = c2.undistortRectifyImage(img2);
	img3 = c3.undistortRectifyImage(img3);

	// Rectification
	Mat newCameraMatrix;
	Mat newimg1, newimg2, newimg3;

	newCameraMatrix = c1.getNewCameraMatrix();

	Mat H23 = Lilo::calcHomography(img2, img3);
	Mat H12 = Lilo::calcHomography(img2, img1);

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

		Mat eye = Mat::eye(3, 3, CV_64F);
		Mat H1 = PTZ * H12;
		Mat H2 = PTZ * H23;
		Mat out1 = Lilo::blend(img1, img2, H1, PTZ);
		Mat outout = Lilo::blend(out1 ,img3, eye, H2);

		//const string test1 = "Test1";
		//namedWindow(test1, WINDOW_NORMAL);
		//imshow( test1, img3 );
		//const string test2 = "Test2";
		//namedWindow(test2, WINDOW_NORMAL);
		//imshow( test2, out1 );
		const string test3 = "Test3";
		namedWindow(test3, WINDOW_NORMAL);
		imshow( test3, outout );
		}while ((key != '\n') && (key != 27));
	std::cout<<"Exiting program"<<std::endl;
	//return 0;
}

