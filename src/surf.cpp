#include <ctime>
#include <opencv2/opencv.hpp>
#include <string>

#include "Camera.hpp"
#include "Lilo.hpp"

using namespace cv;
using namespace std;

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
	Size s = img1.size();
	s.width += 1000;


	Lilo::homographyThreshold = 3;
	Mat out1 = Lilo::stitch(img2, img1, img3, s);
	//Mat H = Lilo::calcHomography(img2, img3);
	//Mat out1 = Lilo::blend(img2, img3, Mat::eye(3, 3, CV_64F), H);

	const string stitch = "Stitching";
	namedWindow(stitch, WINDOW_NORMAL);
	imshow(stitch, out1);

	while (waitKey(0) != '\n');

	return 0;
}
/*
	clock_t start, end;
	start = clock();
	end = clock();

	double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
	cout << elapsed_secs << endl;
*/
