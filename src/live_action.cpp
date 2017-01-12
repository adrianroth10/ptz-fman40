#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "Camera.hpp"
#include "Lilo.hpp"
#include "VirtualCamera.hpp"
#include "MatStruct.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

// Different locations of the data
#ifdef LINUX
const string result_loc = "../results/";
#else
const string result_loc = "../../results/";
#endif
//////////////////////////////////

int main( int argc, char** argv )
{
	Camera c1(1);
	Camera c2(2);

	Mat img1, img2;
	img1 = c1.click();

	//calibration matrices
	Mat cameraMatrix = c1.getNewCameraMatrix();

	//Estimate Stitching homographies
	FileStorage fs;
	fs.open(result_loc + "live_homography.xml", FileStorage::READ);
	if (!fs.isOpened()) {
		string message =
			"Invalid name of camera calibration file";
		throw invalid_argument(message);
	}
	Mat H;
	fs["H"] >> H;

	//Create structures for the alpha-channels
	MatStruct imga1 = MatStruct(img1.size());
	MatStruct imga2 = MatStruct(img1.size());

	const string imgtit="image";
	const string masktit="mask";
	namedWindow(imgtit,WINDOW_NORMAL);
	char key=-1;
	VirtualCamera Vcam(0);
	Mat Rot = Mat::eye(3, 3, CV_64F);
	Mat Zoom = Mat::eye(3, 3, CV_64F);
	Mat eye = Mat::eye(3, 3, CV_64F);
	Mat PTZ;
	do{
		img1 = c1.click();
		img2 = c2.click();
		img1 = c1.undistortRectifyImage(img1);
		img2 = c2.undistortRectifyImage(img2);
		imga1.img = img1;
		imga2.img = img2;

		Vcam.updateView(key, Rot, Zoom);
		PTZ=cameraMatrix * Zoom * Rot * cameraMatrix.inv();
		Mat H1 = PTZ * H;
		MatStruct outout = Lilo::blend(imga1, imga2, H1, PTZ);
		imshow( imgtit, outout.img );
		key=waitKey(30);
		std::cout<<"Key pressed:"<<key<<std::endl;
		}while ((key != '\n') && (key != 27));
	std::cout<<"Exiting program"<<std::endl;
}


