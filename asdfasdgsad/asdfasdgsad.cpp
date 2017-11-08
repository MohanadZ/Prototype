#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>



using namespace cv;
using namespace std;

Mat img; // startscreen Image
Mat img1;
Mat imgTmp; // Tempeary image for the line 
Mat imgTmp2; // Tempeary image for the line 
Mat imgLines; // The line from the detect object
Mat imgOriginal; //Camera image
Mat imgHSV; // First HSV Image
Mat imgHSV2; // Second HSV Image
Mat imgThresholded; // First Threshold image
Mat imgThresholded2; // Second Threshold image
Mat imgBothThresh; // A combined Threshold image
Moments oMoments; // moments ? 
POINT p; // point
POINT q;
POINT a, k;

Mat frame; // Video

int Points= 0;
int i = 0;
int j = 0;
int iLowH; // First low H value
int iHighH; // First High H value
int iLowS; // First Low S Value
int iHighS; // First High S value
int iLowV; // First low V value
int iHighV; // First High V value

int iLowH2; // Second low H value
int iHighH2; // Second High H value
int iLowS2; // Second low S value
int iHighS2; // Second High S value
int iLowV2; // Second low V value
int iHighV2; // Second High V value

int xposition = 0; // X position for the cursor
int yposition = 0; // Y position for the cursor
int time = 0; // Time where the cursor is

int xposition2 = 0; // X position for the cursor
int yposition2 = 0; // Y position for the cursor
int dect = 0;

int xposition3 = 0; // X position for the cursor
int yposition3 = 0;

int xposition4 = 0; // X position for the cursor
int yposition4 = 0;


int iLastX = -1;
int iLastY = -1;


void objectTracking();
void screenshot(POINT a, POINT b);


 int main(int argc, char** argv)
{
	VideoCapture cap(0); //capture the video from webcam
	//CvCapture *capture = cvCreateCameraCapture(0);
	if (!cap.isOpened())  // if not success, exit program
	{
		return -1;
	}

	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280); // Size of the camera
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G')); // set the source, so it runs faster
	
																// Get one frame
	
	
	// picture/ gamewindow
	
	namedWindow("Game", WINDOW_NORMAL);
	setWindowProperty("Game", CV_WND_PROP_FULLSCREEN, 1);
	img = imread("1.jpg",1);
	putText(img, "Score:", Point2d(800, 100), CV_FONT_HERSHEY_COMPLEX, 4, Scalar(0, 0,0,0), 3, 8, false);
	imshow("Game", img);
	
	
	
	namedWindow("Control", CV_WINDOW_FREERATIO); //create a window called "Control"

	
	//img1 = imread("5.png", CV_LOAD_IMAGE_GRAYSCALE);
	//imshow("Test", img1);
	//putText(imgOriginal, "afsafasf", Point(img.cols / 4, img.rows / 2), CV_FONT_HERSHEY_COMPLEX, 20, Scalar(255, 255, 255));

	
	//red
	iLowH = 51;
	iHighH = 84;

	iLowS = 92;
	iHighS = 255;

	iLowV = 27;
	iHighV = 166;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	namedWindow("Control2", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//Green
	iLowH2 =51;
	iHighH2 = 92;

	iLowS2 = 82;
	iHighS2 = 216;

	iLowV2 = 32;
	iHighV2 = 158;

	//Create trackbars in "Control" window
	createTrackbar("LowH2", "Control2", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH2", "Control2", &iHighH, 179);

	createTrackbar("LowS2", "Control2", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS2", "Control2", &iHighS, 255);

	createTrackbar("LowV2", "Control2", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV2", "Control2", &iHighV, 255);


	//Capture a temporary image from the camera
	cap.read(imgTmp);
	
	
	
	//Create a black image with the size as the camera output
	imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;


	while (true)
	{

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{

			break;
		}


		objectTracking();
		POINT a, b;
		a.x = 0;
		a.y = 0;

		b.x = 3000;
		b.y = 3000;

		if (i = 9) {
			screenshot(a, b);
			i = 0;
		}
		else
		{
			i++;
		}
		


		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{

			break;
		}
	
			
	}
		return(0);

	} 

	void screenshot(POINT a, POINT b)
	{
		
		
		// copy screen to bitmap
		HDC     hScreen = GetDC(NULL);
		HDC     hDC = CreateCompatibleDC(hScreen);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, abs(b.x - a.x), abs(b.y - a.y));
		HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
		BOOL    bRet = BitBlt(hDC, 0, 0, abs(b.x - a.x), abs(b.y - a.y), hScreen, a.x, a.y, SRCCOPY);
		

		// save bitmap to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();


	//	WriteFile(hBitmap, L"output.bmp" );
		SetClipboardData(CF_BITMAP, hBitmap);
		CloseClipboard();

		// clean up
		SelectObject(hDC, old_obj);
		DeleteDC(hDC);
		ReleaseDC(NULL, hScreen);
		DeleteObject(hBitmap);
	}


 void objectTracking()
 {

	 cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	 cvtColor(imgOriginal, imgHSV2, COLOR_BGR2HSV);
	 inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image	
	 inRange(imgHSV2, Scalar(iLowH2, iLowS2, iLowV2), Scalar(iHighH2, iHighS2, iHighV2), imgThresholded2); //Threshold the image
	 imgBothThresh = imgThresholded + imgThresholded2;
	 //imshow("Both THresh", imgBothThresh);
	 
	 //morphological opening (removes small objects from the foreground)
	 erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	 dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	 //morphological closing (removes small holes from the foreground)
	 dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	 erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	 //morphological opening (removes small objects from the foreground)
	 erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	 dilate(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	 //morphological closing (removes small holes from the foreground)
	 dilate(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	 erode(imgThresholded2, imgThresholded2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	 
	// SetCursorPos(iLastX, iLastY); // Position of the cursor

	 //Calculate the moments of the thresholded image
	 oMoments = moments(imgBothThresh);

	 double dM01 = oMoments.m01;
	 double dM10 = oMoments.m10;
	 double dArea = oMoments.m00;

	 // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	 if (dArea > 500000)
	 {
		 //calculate the position of the object
		 double posX = dM10 / dArea;
		 double posY = dM01 / dArea;

		 if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		 {
			
			//line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(255, 255, 255), 2);
			if ((GetCursorPos(&p)) && (xposition != q.x +- 100) && (yposition != q.y +- 100) && j < 10) {
					//imgLines.at<uchar>(p.y, p.x) = 255;
					line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(255, 255, 255), 4);
					j++;
			 }
			 else {
				 imshow("zero", imgLines);
				 //imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;
				 //j = 0;
			 }
			 		
			/*if ((GetCursorPos(&p)) && (xposition != p.x) && (yposition != p.y)) {
			line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(255, 255, 255), 2);

			 }
		 else {
			 imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;

		 }*/

			 //Draw a red line from the previous point to the current point
			
		 }

		 iLastX = posX;
		 iLastY = posY;
	 }

	 
	 namedWindow("Test", WINDOW_AUTOSIZE);
	 imshow("Test", imgLines);
	 

	/*if ((GetCursorPos(&p)) && (xposition != p.x +- 5) && (yposition != p.y +- 5) && (i = 10)) {
		
		 namedWindow("Test", WINDOW_AUTOSIZE);
		 imshow("Test", imgLines);
		
		// imshow("Test", imgTmp2);
		 i = 0;
		 
	 }*/
	
	// namedWindow("Test", WINDOW_AUTOSIZE);
	// if ((GetCursorPos(&p)) && (xposition == p.x) && (yposition == p.y))
	 
	 //imshow("Test", imgLines);
	 
	// imgLines

	//SetCursorPos(p.x, p.y); // Position of the cursor
	//SetCursorPos(iLastX, iLastY); // Position of the cursor



	 //imshow("Thresholded Image", imgThresholded); //show the first thresholded image
	 //imshow("Thresholded Image2", imgThresholded2);//show the second thresholded image

	imgOriginal = imgOriginal /*- imgTmp*/ + imgLines;
	imshow("Original", imgOriginal); //show the original image

	

	 //while (true) {

	 if (GetCursorPos(&p) && xposition != p.x && yposition != p.y)
	 {
		 xposition = p.x;
		 yposition = p.y;
		 //cursor position now in p.x and p.y

	 }
	 //cout << "[" << p.x << ", " << p.y << "] " ;

	 //}

	 //if statement to check the time inside area
	 if ((480 < xposition) && (xposition < 744) && (151 < yposition) && (yposition < 252)) {
		 time++;
		 //cout << time;
	 }
	 else {
		 time = 0;
		 
		// cout << time;

	 }
	 // If cursor is inside area and have been there for a few seconds
	/* if ((480 < xposition) && (xposition < 744) && (151 < yposition) && (yposition < 252) && (time >= 20))
	 {
		 namedWindow("Game", CV_WINDOW_FULLSCREEN);

		 // video player
		 namedWindow("Sample Video", 1);

		 VideoCapture cap("319.avi");
		 // if (!cap.isOpened())  // check if we succeeded
			 // return -1;

		 while (1) {
			 cap >> frame;
			 //show("Sample Video", frame);
			 char c = waitKey(33);
			 if (c == 0)
				 break;

		 }

		 		 
	 }*/

	

 }


	//detect specific size of object
	//Make it detect symbols drawn
	// remove background image
	//http://opencv-srf.blogspot.dk/2011/11/mouse-events.html
	//https://en.sfml-dev.org/forums/index.php?topic=8599.0
	//https://www.funprogramming.org/95-Is-the-mouse-inside-a-square.html
	//https://stackoverflow.com/questions/14090906/make-an-object-clickable-click-if-the-mouse-is-in-the-area-of-object
	//http://opencv-srf.blogspot.dk/2011/09/capturing-images-videos.html
