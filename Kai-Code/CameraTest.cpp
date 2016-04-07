// This code is my sample code that I am uploading to the Edison to prove that my OpenCV installation is functioning properly. 
// Currently, when running 'make CameraTest' while in the opencv-3.1.0/samples directory, I get an error that states,
// fatal error: opencv2/opencv.hpp: No such file or directory #include <opencv2/opencv.hpp>"
// That .hpp file does indeed exist in opencv-3.1.0/include/opencv2/opencv.hpp


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
