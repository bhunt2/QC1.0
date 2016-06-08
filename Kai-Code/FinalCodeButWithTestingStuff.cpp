// This code is helpful for testing out certain functions of the object tracking code. It's mostly the same as the ImageProcessing_FinalCode
// file but with a few additional pieces of code for testing purposes.
// Hitting 'b' after camshift has created a bounding box will change the display of the image to the backprojection of the image. This is useful
// for determining whether or not a good match can be found. White pixels represent a good match for the hsv of the object you wish to track,
// black pixels are filtered out and are not a good match. Adjusting the Vmin and Smin sliders on the output window will help change which objects
// will be tracked and what will not. This may change based off of the hsv of the object to track and lighting of the environment.

#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <ctype.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <numeric>

using namespace cv;
using namespace std;

// Global Variables
Mat frame, image, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
Rect selection, trackWindow;
int trackObject = -1;
bool backprojMode = false;
bool showHist = true;
int vmin = 20, vmax = 256, smin = 170;
int previousSelectionHeight = 0;
int runTimeCounter = 0;
double previousDistance = 0, runTimeTotal = 0;
string velocity;
float runTime = 0;

struct distanceCalculations {
	double distance;
	double velocity;
	double height;
} parameters;

// Function headers
int main();
int WriteFile();
long startTimer();
long stopTimer();
float getRunTime(clock_t, clock_t);
void undistortCamera(Mat&, Mat&);
void objectTracking(Mat&, Rect&, Rect&);
void MatchingMethod(Mat&, Rect&, void*);
void calculateModel(distanceCalculations, float, Rect);
VideoCapture getCamera();

int main()
{
	VideoCapture cap = getCamera();

	if (!cap.isOpened())
	{
		cout << "***Could not initialize capturing...***\n";
		return -1;
	}
	// Creates frame dimensions. Same as aspect ratio of camera
	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// Windows and trackbars for displaying images and adjusting sliders for tracking objects
	// Vmax should not be adjusted. Adjusting Vmin and Smin can give a better match for camshift tracking depending on the hsv of the object,
	// and the lighting of the environment. 
	namedWindow("Histogram", 0);
	namedWindow("CamShift Demo", 0);
	createTrackbar("Vmin", "CamShift Demo", &vmin, 256, 0);
	createTrackbar("Vmax", "CamShift Demo", &vmax, 256, 0);
	createTrackbar("Smin", "CamShift Demo", &smin, 256, 0);

	// Writes output video of testing. This will output to where the executable is located for this program.
	VideoWriter video("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 6, Size(frame_width, frame_height), true);
    // Loop as long as video feed is established
	for(;;)
    {
        // Continue camera stream and display image frames
        cap >> frame;
        if( frame.empty() )
            break;

		// Start timer necessary to know iteration time between previous frame and current frame (frames per second)
		clock_t startTime = startTimer();
		
		// Get rid of fisheye distortion
		undistortCamera(frame, image);

		frame.copyTo(image);
		
		// Uses template matching to find input uploaded image of object in video stream of camera
		if (trackWindow.x == 0 || trackWindow.width > 400 || trackWindow.width > trackWindow.height || trackWindow.height > 350) // || trackWindow.height > 350)
		{
			MatchingMethod(image, selection, 0);
		}
		
		// Camshift object tracking
		objectTracking(image, selection, trackWindow);
		
		// Calculate distance, velocity, and direction of movement parameters for the object being tracked
		if (runTime != 0)
		{
			calculateModel(parameters, runTime, trackWindow);
		}

		// Writes image to video output
		video.write(image);
		// Displays images as windows
		imshow("CamShift Demo", image);
		imshow("Histogram", histimg);

		// If esc key is pressed, terminates program
		char c = (char)waitKey(1);
		
		// Stop timer necessary to know iteration time between previous frame and current frame (frames per second)
		clock_t stopTime = stopTimer();

		// Calculates code run time for one iteration
		runTime = getRunTime(startTime, stopTime);
		if (c == 27)
			break;
		switch (c)
		{
		case 'b':
			backprojMode = !backprojMode;
			break;
		case 'c':
			trackObject = 0;
			histimg = Scalar::all(0);
			break;
		case 'h':
			showHist = !showHist;
			if (!showHist)
				destroyWindow("Histogram");
			else
				namedWindow("Histogram", 1);
			break;
		/*case 'p':
			paused = !paused;
			break;*/
		default:
			;
		}
    }
    return 0;
}

VideoCapture getCamera()
// Function to open camera stream
{
	VideoCapture cap;
	cap.open(0);

	return cap;
}

void undistortCamera(Mat& frame, Mat& image)
// Function to remove the fisheye distortion of our camera
{
	// Constants determined from camera calibration to eliminate fisheye distortion
	Mat cameraMatrix = (Mat_<double>(3, 3) << 3.9683218974894680e+02, 0., 3.1950000000000000e+02, 0.,
		3.9683218974894680e+02, 2.3950000000000000e+02, 0., 0., 1.);
	Mat distCoeffs = (Mat_<double>(5, 1) << -4.5596899020020748e-01, 2.5706133556827276e-01, 0., 0.,
		-7.4662921360934026e-02);
	
	Mat R, map1, map2, undistortedframe;

	// Functions to remove fisheye distortion
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, Matx33d::eye(), cameraMatrix, Size(640, 480), CV_16SC2, map1, map2);
	cv::remap(frame, image, map1, map2, CV_INTER_LINEAR);	
}

void calculateModel(distanceCalculations, float runTime, Rect trackWindow)
// Function to calculate distance, velocity, and direction of movement parameters for the object being tracked
{
	// focal_length = focal length given by manufacturer in mm. 
	// f_x and f_y values (pixels) in CameraMatrix output that was generated by CameraCalibration code are same value so I simplified to just f.
	// actual_object_height is the real world height of the object in mm. Hardcoded for testing purposes.
	double focal_length = 2.1, f = 396.83218974894680, actual_object_height = 1700;
	// camera resolution is 640x480 by default. Camera resolution can be increased but performance suffers. Aspect ratio is 640x480.
	int camera_resolution_x = 640, camera_resolution_y = 480, aspect_ratio_x = 640, aspect_ratio_y = 480;
	// scaling factor that scales the camera calibration focal length to the manufacturer focal length
	double scale_factor = f / focal_length;
	// pixels/mm after being scaled down to the 640x480 aspect ratio
	double scaled_px_per_mm = (aspect_ratio_x * scale_factor) / camera_resolution_x;
	// Determines the object height in pixels as it appears in the image
	double object_height_image = trackWindow.height / scaled_px_per_mm;
	// Distance from object to camera in mm from previous frame
	previousDistance = parameters.distance;
	// Unit = mm. Distance from object to camera from current frame
	parameters.distance = (actual_object_height * focal_length) / object_height_image;
	// Unit = m/s. Velocity is calculated by taking the distance difference between two frames and the run time through one iteration of code.
	parameters.velocity = (parameters.distance - previousDistance) / (runTime);
	// Create vector to store velocity values for half a second of sampling.
	vector<double> velocityList;
	velocityList.push_back(parameters.velocity);
	runTimeTotal = runTimeTotal + runTime;
	// After half a second of sampling, calculate the average velocity over the sample size
	if (runTimeTotal > 500)
	{
		double velocityAverage = std::accumulate(velocityList.begin(), velocityList.end(), 0.0) / runTimeCounter;
		runTimeCounter = 0;
		runTimeTotal = 0;
		velocityList.clear();
		velocity = to_string(velocityAverage);
	}
	// Unit = mm. Passes in height of object to parameters.height
	parameters.height = actual_object_height;

	// Displays data on video stream. putText shows the data written onto the image itself.
	string distance = to_string(parameters.distance * (1 / 304.8));
	putText(image, distance + " ft.", Point(5, 475), FONT_HERSHEY_SIMPLEX, 2, Scalar::all(255), 3, 8);
	putText(image, velocity + " m/s.", Point(5, 425), FONT_HERSHEY_SIMPLEX, 2, Scalar::all(255), 3, 8);

	// outputs data to console if you wish to read it there.
	if (previousSelectionHeight != trackWindow.height)
	{
		cout << "Height of object in image is: " << trackWindow.height << " pixels" << endl;
		cout << "The width of the object is: " << trackWindow.width << " pixels" << endl;
		cout << "Distance from object to camera is: " << parameters.distance * (1/304.8) << " ft" << endl;
		cout << "Run Time is: " << runTime << " ms." << endl;
		cout << "Velocity of object is: " << parameters.velocity << " m/s" << endl;
		previousSelectionHeight = trackWindow.height;
		WriteFile();
	}
}

long startTimer()
// Function to start run time timer
{
	clock_t startTime = clock();
	return startTime;
}

long stopTimer()
// Function to stop run time tiemr
{
	clock_t stopTime = clock();
	return stopTime;
}

float getRunTime(clock_t startTime, clock_t stopTime)
// Function to get run time of one iteration of code
{
	float runTime = stopTime - startTime;
	runTimeCounter++;
	return runTime;
}

void objectTracking(Mat& image, Rect& selection, Rect& trackWindow)
// Function to track the object in the camera stream using Camshift
{
	int hsize = 16;
	float hranges[] = { 0,180 };
	const float* phranges = hranges;
	
	cvtColor(image, hsv, COLOR_BGR2HSV);

	if (trackObject)
	{
		int _vmin = vmin, _vmax = vmax;

		inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
			Scalar(180, 256, MAX(_vmin, _vmax)), mask);
		int ch[] = { 0, 0 };
		hue.create(hsv.size(), hsv.depth());
		mixChannels(&hsv, 1, &hue, 1, ch, 1);

		if (trackObject < 0)
		{
			Mat roi(hue, selection), maskroi(mask, selection);
			calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);

			trackWindow = selection;
			trackObject = 1;

			histimg = Scalar::all(0);
			int binW = histimg.cols / hsize;
			Mat buf(1, hsize, CV_8UC3);
			for (int i = 0; i < hsize; i++)
				buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180. / hsize), 255, 255);
			cvtColor(buf, buf, COLOR_HSV2BGR);

			for (int i = 0; i < hsize; i++)
			{
				int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
				rectangle(histimg, Point(i*binW, histimg.rows),
					Point((i + 1)*binW, histimg.rows - val),
					Scalar(buf.at<Vec3b>(i)), -1, 8);
			}
		}

		calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
		backproj &= mask;
		RotatedRect trackBox = CamShift(backproj, trackWindow,
			TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
		if (trackWindow.area() <= 1)
		{
			int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
			trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
				trackWindow.x + r, trackWindow.y + r) &
				Rect(0, 0, cols, rows);
		}
		if (backprojMode)
			cvtColor(backproj, image, COLOR_GRAY2BGR);
		ellipse(image, trackBox, Scalar(0, 0, 255), 3, LINE_AA);
	}
}

void MatchingMethod(Mat& image, Rect& selection, void*)
// Code to match the object captured in the uploaded image to the object located in the camera stream
{
	// Initialize variables
	Mat img_display, templ, result;
	image.copyTo(img_display);
	// Read in input image
	templ = imread("C:/Users/Public/Desktop/Test.jpg");

	/// Create the result matrix
	int result_cols = image.cols - templ.cols + 1;
	int result_rows = image.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	// Do the Matching and Normalize
	// "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	int match_method = 0;
	matchTemplate(image, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());


	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	// Create bounding box around matched object
	selection.x = matchLoc.x;
	selection.y = matchLoc.y;
	selection.width = templ.cols;
	selection.height = templ.rows;

	rectangle(image, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 255, 0), 2);
	// Reset trackbox each time template matching needs to be called
	trackObject = -1;
}

int WriteFile() 
{
	ofstream outputFile;
	string Path = "C:/Users/Public/Desktop/ParameterOutput.txt";
	// Checks if file exists. If it does, open file. If it doesn't, create file
	if (outputFile.good())
	{
		// Opens file and appends to it, keeping a running log of all values calculated
		outputFile.open(Path, std::ios::app);
		// Uncomment to open file and rewrites values, keeping only the most recent value in the txt file
		//outputFile.open(Path);
	}
	else
	{
		// Creates file at the target path if file doesn't already exists
		ofstream outputFile(Path);
	};
	// Writes parameters to file
	if (outputFile.is_open())
	{
		outputFile << "Distance from object to camera is: " << parameters.distance * (1 / 304.8) << " ft." << "\n";
		outputFile << "Velocity of object is: " << velocity << " m/s." << "\n";
		outputFile.close();
	}
	else cout << "Unable to open file";
	return 0;
}
