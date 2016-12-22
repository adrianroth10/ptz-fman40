#include "curl/curl.h" // has to go before opencv headers

#include <iostream>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

//curl writefunction to be passed as a parameter
// we can't ever expect to get the whole image in one piece,
// every router / hub is entitled to fragment it into parts
// (like 1-8k at a time),
// so insert the part at the end of our stream.
size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    vector<uchar> *stream = (vector<uchar>*)userdata;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    return count;
}

//function to retrieve the image as cv::Mat data type
cv::Mat curlImg(const char *img_url, int timeout=10)
{
    vector<uchar> stream;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, img_url); //the img url
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); // timeout if curl_easy hangs, 
    curl_easy_perform(curl); // start curl
    curl_easy_cleanup(curl); // cleanup
    return imdecode(stream, -1); // 'keep-as-is'
}

int main(void)
{
	Mat image;
    //Mat image = curlImg("http://www.cars.co.za/images/pictures/general/graphic_sellyourcar.png");
    //if (image.empty())
      //  return -1; // load fail
	
	//const string address= "http://root@31.208.80.26:81/axis-cgi/mjpg/video..cgi?user=root&password=ptz-fman40";
	/*cv::VideoCapture vCap(address);
	if (!vCap.isOpened())
	{
		cout<<"Failed to open stream"<<endl;
		return -1;
	}
	vCap >> image;
	*/
	const char address[]{"wget --user=root --password=ptz-fman40 -O test.jpg 31.208.80.26:81/axis-cgi/jpg/image.cgi"};
	system(address);
	image=imread("test.jpg");	 

	namedWindow( "Image output", CV_WINDOW_AUTOSIZE );
	imshow("Image output",image); // here's your car ;)
	waitKey(0); // infinite
}
