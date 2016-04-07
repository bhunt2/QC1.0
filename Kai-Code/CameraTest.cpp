
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