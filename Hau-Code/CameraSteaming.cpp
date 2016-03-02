#include <opencv2/opencv.hpp>
#include <iostream>
int main(int, char**)
{
	//Open the first webcam plugged in the computer
	cv::VideoCapture camera(1);
	if (!camera.isOpened())
	{
		std::cerr << "ERROR: Could not open camera" << std::endl;
		return 1;
	}

	//Create a window to display the images from the webcam
	cv::namedWindow("Webcam", CV_WINDOW_AUTOSIZE);

	//Pick up a new frame and display it until you press a key
	while (1)
	{
		//This will contain the image from the webcam
		cv::Mat frame;

		//Capture the next frame from the webcam
		camera >> frame;

		//Show the image on the window
		cv::imshow("Webcam", frame);

		//Wait for a key to be pressed
		if (cv::waitKey(30) >= 0) break;
	}

	//Success. 
	return 0;
}