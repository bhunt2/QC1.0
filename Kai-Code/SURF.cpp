#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

//void readme();

/** @function main */
int main(int argc, char** argv)
{
	VideoCapture capture;
	Mat frame;
	//-- 2. Read the video stream
	capture.open(0);
	int frame_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	VideoWriter video("SURFoutput.avi", CV_FOURCC('M', 'J', 'P', 'G'), 6, Size(frame_width, frame_height), true);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		
		//-- Step 1: Detect the keypoints using SURF Detector
		int minHessian = 400;

		Ptr<SURF> detector = SURF::create(minHessian);

		std::vector<KeyPoint> keypoints_1, keypoints_2;

		detector->detect(frame, keypoints_1);

		//-- Draw keypoints
		Mat img_keypoints_1;

		drawKeypoints(frame, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

		//-- Show detected (drawn) keypoints
		video.write(img_keypoints_1);
		imshow("Keypoints 1", img_keypoints_1);

		int c = waitKey(10);
		if ((char)c == 27) { break; } // escape
	}
	return 0;
}