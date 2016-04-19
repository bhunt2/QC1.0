
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;

int main() 
{
	VideoCapture cap(1);
	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	if (!cap.isOpened()) 
	{
		cout << "I'm blind..." << endl;
		return -1;
	}
	cout << "I can see!" << endl;
	Mat cameraMatrix = (Mat_<double>(3, 3) << 3.9683218974894680e+02, 0., 3.1950000000000000e+02, 0.,
		3.9683218974894680e+02, 2.3950000000000000e+02, 0., 0., 1.);
	Mat distCoeffs = (Mat_<double>(5, 1) << -4.5596899020020748e-01, 2.5706133556827276e-01, 0., 0.,
		- 7.4662921360934026e-02);
	Mat R, map1, map2, frame, undistortedframe;
	for (;; ) {
		Mat frame;
		cap >> frame;
		imshow("Fisheye Frame", frame);
		cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, Matx33d::eye(), cameraMatrix, Size(640, 480), CV_16SC2, map1, map2);
		cv::remap(frame, undistortedframe, map1, map2, CV_INTER_LINEAR);
		imshow("Undistorted Frame", undistortedframe);
		char c = (char)waitKey(33);
		if (c == 27) break;
	}
	return 0;
}