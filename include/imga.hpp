#ifndef IMGA_H
#define IMGA_H

#include <opencv2/opencv.hpp>
struct imga{
	cv::Mat img;
	cv::Mat mask;
	imga(cv::Size s){
		mask=cv::Mat(s,CV_32FC3);
		mask.setTo(1.0);
	}
};


#endif
