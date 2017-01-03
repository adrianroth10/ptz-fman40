#include <curl/curl.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "Camera.hpp"


Mat curlImg(const char *img_url, int timeout = 10);
size_t write_data(char *ptr,
		  size_t size,
		  size_t nmemb,
		  void *userdata);

using namespace cv;
using namespace std;

// Different locations of the data
#ifdef LINUX
const string data_loc = "../data/";
#else
const string data_loc = "../../data/";
#endif
//////////////////////////////////

Camera::Camera(int nbr)
{
	switch (nbr) {
		case -3:
			camera_file = data_loc + "camera_football.xml";
			source = data_loc + "20150521_194353_49E3.jpg";
			break;
		case -2:
			camera_file = data_loc + "camera_football.xml";
			source = data_loc + "20150521_194353_FD1E.jpg";
			break;
		case -1:
			camera_file = data_loc + "camera_football.xml";
			source = data_loc + "20150521_194353_C1D8.jpg";
			break;
		case 1:
			camera_file = data_loc + "out_camera_1.xml";
			source = "83.233.133.248:81/axis-cgi/jpg/image.cgi?camera=1";
			break;
		case 2:
			camera_file = data_loc + "out_camera_2.xml";
			source = "83.233.133.248:81/axis-cgi/jpg/image.cgi?camera=2";
			break;
		default:
			throw invalid_argument("Not a valid camera");
	}

	Mat cameraMatrix,
	    distortCoefficients,
	    img;

	this->source = source;
	this->camera_file = camera_file;
	readFile(cameraMatrix, distortCoefficients);
	img = click();

	newCameraMatrix = cameraMatrix;
		//getOptimalNewCameraMatrix(cameraMatrix,
						    //distortCoefficients,
						    //img.size(),
						    //1.0,
						    //img.size());
	initUndistortRectifyMap(cameraMatrix,
			        distortCoefficients,
			        noArray(),
			        cameraMatrix,
			        img.size(),
			        CV_32FC1,
			        rectmap1,
			        rectmap2);
}

Mat Camera::click()
{
	Mat output;
	if (camera_file.find("camera_football.xml") != string::npos) {
		output = imread(source, CV_LOAD_IMAGE_COLOR);
	} else {
		output = curlImg(source.c_str());
	}
	return output;
}

//function to retrieve the image as cv::Mat data type
Mat curlImg(const char *img_url, int timeout)
{
	vector<uchar> stream;
	CURL *curl = curl_easy_init();
	//the img url
	curl_easy_setopt(curl, CURLOPT_URL, img_url);
	curl_easy_setopt(curl, CURLOPT_USERPWD, "root:pass");
	// pass the writefunction
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	// pass the stream ptr to the writefunction
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);
	// timeout if curl_easy hangs,
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
	curl_easy_perform(curl); // start curl
	curl_easy_cleanup(curl); // cleanup
	return imdecode(stream, -1); // 'keep-as-is'
}

Mat Camera::getNewCameraMatrix()
{
	return newCameraMatrix;
}

void Camera::readFile(Mat &cameraMatrix, Mat &distortCoefficients)
{
	FileStorage fs;
	fs.open(camera_file, FileStorage::READ);
	if (!fs.isOpened()) {
		string message =
			"Invalid name of camera calibration file";
		throw invalid_argument(message);
	}

	FileNode n;
	n = fs["opencv_storage"];

	n["Camera_Matrix"] >> cameraMatrix;
	n["Distortion_Coefficients"] >> distortCoefficients;
}

Mat Camera::undistortRectifyImage(Mat input)
{
	Mat output;

	remap(input,
	      output,
	      rectmap1,
	      rectmap2,
	      INTER_LINEAR);


	return output;
}

// curl writefunction to be passed as a parameter
// we can't ever expect to get the whole image in one piece,
// every router / hub is entitled to fragment it into parts
// (like 1-8k at a time),
// so insert the part at the end of our stream.
size_t write_data(char *ptr,
		  size_t size,
		  size_t nmemb,
		  void *userdata)
{
	vector<uchar> *stream = (vector<uchar>*)userdata;
	size_t count = size * nmemb;
	stream->insert(stream->end(), ptr, ptr + count);
	return count;
}
