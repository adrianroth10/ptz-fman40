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
	imwrite(result_loc + "rect_1.jpg", img1);
	imwrite(result_loc + "rect_2.jpg", img2);
	imwrite(result_loc + "rect_3.jpg", img3);
	Size s = img1.size();


	// Writing stitched stuff with different blending funcs
	string out_loc_1 = result_loc + "stitch_linear.jpg";
	string out_loc_2 = result_loc + "stitch_sigmoid.jpg";
	Mat out1 = Lilo::stitch(img2, img1, s, LINEAR);
	Mat out2 = Lilo::stitch(img2, img1, s, SIGMOID);
	imwrite(out_loc_1, out1);
	imwrite(out_loc_2, out2);

	// Writing just homography stuff
	Mat H1 = Mat::eye(3, 3, CV_64F);
	Mat H2 = Mat::eye(3, 3, CV_64F);
	H1.at<double>(0, 2) = 1050;
	H2.at<double>(0, 2) = -1050;
	Mat H = Lilo::calcHomography(img2, img1);
	warpPerspective(img1, out1, H1 * H, out1.size());
	imwrite(result_loc + "stitch_warped.jpg", out1);

	// writing out blending stuff
	s.width += 1050;

	MatStruct imga1 = MatStruct(img1.size());
	MatStruct imga2 = MatStruct(img2.size());
	Mat white1 = imga1.mask;
	Mat white2 = imga2.mask;
	Mat whiteOut1(s, CV_32FC3);
	Mat whiteOut2(s, CV_32FC3);
	Lilo::getBlendingMats(whiteOut1,
			      whiteOut2,
			      white1,
			      white2,
			      H1 * H,
			      H1,
			      SIGMOID);
	warpPerspective(img1, out1, H1 * H, s);
	warpPerspective(img2, out2, H1, s);
	multiply(out1, whiteOut1, out1, 1, CV_8UC3);
	multiply(out2, whiteOut2, out2, 1, CV_8UC3);
	warpPerspective(out1, out1, Mat::eye(3, 3, CV_64F), img1.size());
	warpPerspective(out2, out2, H2, img2.size());
	imwrite(result_loc + "stitch_sigmoid_1.jpg", out1);
	imwrite(result_loc + "stitch_sigmoid_2.jpg", out2);

	Lilo::getBlendingMats(whiteOut1,
			      whiteOut2,
			      white1,
			      white2,
			      H1 * H,
			      H1,
			      LINEAR);
	warpPerspective(img1, out1, H1 * H, s);
	warpPerspective(img2, out2, H1, s);
	multiply(out1, whiteOut1, out1, 1, CV_8UC3);
	multiply(out2, whiteOut2, out2, 1, CV_8UC3);
	warpPerspective(out1, out1, Mat::eye(3, 3, CV_64F), img1.size());
	warpPerspective(out2, out2, H2, img2.size());
	imwrite(result_loc + "stitch_linear_1.jpg", out1);
	imwrite(result_loc + "stitch_linear_2.jpg", out2);

	out1 = Lilo::stitch(img2, img1, s, LINEAR);
	out2 = Lilo::stitch(img2, img1, s, SIGMOID);
	imwrite(result_loc + "stitch_linear_big.jpg", out1);
	imwrite(result_loc + "stitch_sigmoid_big.jpg", out2);

	return 0;
}
