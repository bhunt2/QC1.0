//This simple code opens the camera stream, initializes the necessary parameters using Hau's camera calibration code and the output
//XML file that is generated, runs the necessary functions to correct the fisheye distortion, and shows the user the undistorted image.

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;

int main() 
{
	//Opens camera stream. 0 is used for webcam or if the camera is the only usb device. 1 is used if a webcam or another device is
	//present.
	VideoCapture cap(1);
	//dimensions the camera frame
	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	//checks if camera opened properly and reports back a somewhat unnecessary ouput message
	if (!cap.isOpened()) 
	{
		cout << "I'm blind..." << endl;
		return -1;
	}
	cout << "I can see!" << endl;
	//initialize variables using parameters found in output XML file after running Hau's camera calibration code
	Mat cameraMatrix = (Mat_<double>(3, 3) << 3.9683218974894680e+02, 0., 3.1950000000000000e+02, 0.,
		3.9683218974894680e+02, 2.3950000000000000e+02, 0., 0., 1.);
	Mat distCoeffs = (Mat_<double>(5, 1) << -4.5596899020020748e-01, 2.5706133556827276e-01, 0., 0.,
		- 7.4662921360934026e-02);
	Mat R, map1, map2, frame, undistortedframe;
	//loop that runs until user hits escape key. Loop grabs next frame, runs initUndistortRectifyMap and
	//remap functions using the above parameters to correct the fisheye, and then displays the image to the user
	for (;; ) {
		Mat frame;
		cap >> frame;
		//This imshow shows the user the image before any fisheye correction. comment out if you have no desire to see that.
		imshow("Fisheye Frame", frame);
		//functions to remove fisheye distortion. See documentation on camera calibration as to their specifics.
		cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, Matx33d::eye(), cameraMatrix, Size(640, 480), CV_16SC2, map1, map2);
		cv::remap(frame, undistortedframe, map1, map2, CV_INTER_LINEAR);
		//displays video frame of camera stream after correcting fisheye.
		imshow("Undistorted Frame", undistortedframe);
		//break if escape key is hit
		char c = (char)waitKey(33);
		if (c == 27) break;
	}
	return 0;
}
