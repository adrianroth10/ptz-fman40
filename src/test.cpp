#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int, char**) {
	cv::VideoCapture vcap;
	cv::Mat image;

	const std::string videoStreamAddress = "http://<root:ptz-fman40>@<31.208.80.26:80>";

	//open the video stream and make sure it's opened
	if(!vcap.open(videoStreamAddress)) {
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}

	for(;;) {
		if(!vcap.read(image)) {
			std::cout << "No frame" << std::endl;
			cv::waitKey();
		}
		cv::imshow("Output Window", image);
		if(cv::waitKey(1) >= 0) break;
	}
}
