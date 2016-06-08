// This finds the contours of objects in the frame. Slider can be adjusted to increase and decrease the sensitivity of the code. Increasing
// sensitivity increases the how many finer contours are drawn but also decreases performance and vice versa for decreasing sensitivity.
// The idea was to mask anything that was not within the region for completed contours, so a person will be filled in pixels and the background
// area would be masked. Currently, the mask is working backwards and the filled in region is being masked while the background is not. It
// should not take long to change this.

// This code may be modified and used for additional filtering if the hsv filtering in camshift is not enough. I have not tested how drastically this
// may change performance.

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

VideoCapture cap;
Mat frame;
Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
	cap.open(0);

	if (!cap.isOpened())
	{
		cout << "no can find" << endl;
	}


	// Creates frame dimensions. Same as aspect ratio of camera
	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// Loop as long as video feed is established
	for (;;)
	{
		// Continue camera stream if not paused and display frames
		cap >> frame;
		if (frame.empty())
			break;
		frame.copyTo(src);
		/// Convert image to gray and blur it
		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		blur(src_gray, src_gray, Size(3, 3));

		/// Create Window
		const char* source_window = "Source";
		namedWindow(source_window, WINDOW_AUTOSIZE);
		imshow(source_window, src);

		createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
		thresh_callback(0, 0);

		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}
  return(0);
}

/**
 * @function thresh_callback
 */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  int morph_elem = 0;
  int morph_size = 0;
  Mat otherDrawing;
  //Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

  Mat element(5, 5, CV_8U, cv::Scalar(1));
  Mat closed;
  morphologyEx(canny_output, closed, MORPH_CLOSE, element);
  imshow("Morphology", closed);
  /// Find contours
  findContours(closed, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( closed.size(), CV_8UC3 );
  //Mat mask = Mat::zeros(image.shape[:2], np.uint8)
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, CV_FILLED, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
  //imshow("Morphology", otherDrawing);
}
