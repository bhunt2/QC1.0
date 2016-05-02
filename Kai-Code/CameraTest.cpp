// This code is my sample code that I am uploading to the Edison to prove that my OpenCV installation is functioning properly. 
// This code simply checks whether or not a camera is detected. The fact that his code uses OpenCV specific headers also helps to
// verify that OpenCV is correctly installed.

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() 
{
	VideoCapture cap(0);
	if (!cap.isOpened()) 
	{
		cout << "I'm blind..." << endl;
		return -1;
	}
	cout << "I can see!" << endl;
	return 0;
}
