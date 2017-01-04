#include <opencv2/opencv.hpp>
#include <string>

#include "Camera.hpp"
#include "Lilo.hpp"

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
	//s.width += 2000;


	Lilo::homographyThreshold = 3;
	Mat out1 = Lilo::stitch(img2, img1, s, LINEAR);
	Mat out2 = Lilo::stitch(img2, img1, s, SIGMOID);

	string out_loc_1 = result_loc + "stitch_linear.jpg";
	string out_loc_2 = result_loc + "stitch_sigmoid.jpg";
	cout << out_loc_1 << endl;
	cout << out_loc_2 << endl;
	imwrite(out_loc_1, out1);
	imwrite(out_loc_2, out2);

	const string lin = "Linear";
	namedWindow(lin, WINDOW_NORMAL);
	imshow(lin, out1);
	const string sig = "Sigmoid";
	namedWindow(sig, WINDOW_NORMAL);
	imshow(sig, out2);

	while (waitKey(0) != '\n');

	return 0;
}
