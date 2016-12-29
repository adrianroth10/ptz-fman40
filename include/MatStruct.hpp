#ifndef MATSTRUCT_H
#define MATSTRUCT_H

#include <opencv2/opencv.hpp>
class MatStruct{
	public:
		cv::Mat img;
		cv::Mat mask;
		MatStruct(cv::Size s)
		{
			mask=cv::Mat(s,CV_32FC3);
			mask.setTo(1.0);
		}
};


#endif
