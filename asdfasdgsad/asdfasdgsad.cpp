#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	int tempx = 0, tempy = 0, t = 0;

	VideoCapture cap(0); //capture the video from webcam

	if (!cap.isOpened())  // if not success, exit program
	{
		return -1;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280); // Size of the camera
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G')); // set the source, so it runs faster
	
	// picture/ gamewindow
	namedWindow("Game", CV_WINDOW_FULLSCREEN);
	//putText(result, "Start The Game", cvPoint(500,500))
	Mat img = imread("1.jpg");
	imshow("Game", img);
	
	
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 21;

	int iLowS = 95;
	int iHighS = 255;

	int iLowV = 185;
	int iHighV = 243;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;


	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video



		if (!bSuccess) //if not success, break loop
		{

			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

																									  //morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 10000)
		{
			//calculate the position of the ball
			double posX = dM10 / dArea;
			double posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				//Draw a red line from the previous point to the current point
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
			}

			iLastX = posX;
			iLastY = posY;
		}

		//SetCursorPos(iLastX, iLastY); // Position of the cursor



		imshow("Thresholded Image", imgThresholded); //show the thresholded image

		imgOriginal = imgOriginal + imgLines;
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{

			break;
		}

		//while (true) {
		POINT p;
		if (GetCursorPos(&p) && tempx != p.x && tempy != p.y)
		{
			tempx = p.x;
			tempy = p.y;
			//cursor position now in p.x and p.y

		}
		//cout << p.x << ",," << p.y << ",," ;

	//}
		
		//if statement to check the time inside area
		if ((480 < tempx) && (tempx < 744) && (151 < tempy) && (tempy < 252)) {
			t++;
			cout << t;
		}
		else {
			t = 0;

			cout << t;

		}
			// If cursor is inside area and have been there for a few seconds
			if ((480 < tempx) && (tempx < 744) && (151 < tempy) && (tempy < 252) && (t >= 70))
			{
				namedWindow("Game", CV_WINDOW_NORMAL);
				Mat img2 = imread("max.jpg");
				imshow("Game", img2);

			}
		
	}
	

	

	
	
	
	
		return(0);

	} 



	//detect specific size of object
	//Make it detect symbols drawn
	//http://opencv-srf.blogspot.dk/2011/11/mouse-events.html
	//https://en.sfml-dev.org/forums/index.php?topic=8599.0
	//https://www.funprogramming.org/95-Is-the-mouse-inside-a-square.html
	//https://stackoverflow.com/questions/14090906/make-an-object-clickable-click-if-the-mouse-is-in-the-area-of-object

