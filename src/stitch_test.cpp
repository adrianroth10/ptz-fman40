#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>

using namespace cv;
using namespace std;


#ifdef LINUX
const string data_loc = "../data/";
#else
const string data_loc = "../../data/";
#endif

int main()
{
	vector< Mat > vImg;
	Mat img1, img2, img3, rImg;

	img1 = imread(data_loc+"20150521_194353_49E3.jpg", 1);
	img2 = imread(data_loc+"20150521_194353_FD1E.jpg", 1);
	img3 = imread(data_loc+"20150521_194353_C1D8.jpg", 1);

	vImg.push_back(img1);
	vImg.push_back(img2);
	vImg.push_back(img3);

	Stitcher stitcher = Stitcher::createDefault();

	unsigned long AAtime=0, BBtime=0; //check processing time
	AAtime = getTickCount(); //check processing time

	Stitcher::Status status = stitcher.stitch(vImg, rImg);

	BBtime = getTickCount(); //check processing time
	printf("%.2lf sec \n",  (BBtime - AAtime)/getTickFrequency() ); //check processing time

	if (Stitcher::OK == status) {
		const string stitching_window = "Stitching";
		namedWindow(stitching_window, WINDOW_NORMAL);
		imshow(stitching_window,rImg);
	} else
		printf("Stitching fail.");

	while (waitKey(0) != '\n');
}
