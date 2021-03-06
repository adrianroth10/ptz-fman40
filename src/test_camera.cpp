#include <opencv2/opencv.hpp>
#include <string>

#include "Camera.hpp"
#include "Lilo.hpp"
#include "MatStruct.hpp"

using namespace cv;
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
	img2 = c2.click();
	img1 = c1.undistortRectifyImage(img1);
	img2 = c2.undistortRectifyImage(img2);

	Lilo::homographyThreshold = 3;
	Mat H = Lilo::calcHomography(img2, img1);
	Mat e = Mat::eye(3, 3, CV_64F);
	MatStruct imga1=MatStruct(img1.size());
	MatStruct imga2=MatStruct(img2.size());
	imga1.img = img1;
	imga2.img = img2;
	MatStruct out1 = Lilo::blend(imga1, imga2, H, e);

	const string cam1 = "Camera 1";
	namedWindow(cam1, WINDOW_NORMAL);
	imshow(cam1, img1);
	const string cam2 = "Camera 2";
	namedWindow(cam2, WINDOW_NORMAL);
	imshow(cam2, img2);
	const string stitch = "Stitch";
	namedWindow(stitch, WINDOW_NORMAL);
	imshow(stitch, out1.img);

	imwrite(result_loc + "camera_1.jpg", img1);
	imwrite(result_loc + "camera_2.jpg", img2);
	imwrite(result_loc + "home_made_stitch.jpg", out1.img);

	FileStorage fs;
	fs.open(result_loc + "live_homography.xml", FileStorage::WRITE);
	if (!fs.isOpened()) {
		string message =
			"Invalid name of camera calibration file";
		throw invalid_argument(message);
	}

	fs << "H" << H;

	while (waitKey(0) != '\n');

	return 0;
}
