#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat imgOriginal;
Mat imgTmp;
Mat imgLines;
Mat imgHSV;
Mat imgThresholded;
Mat templateArray[] = { imread("Shapes1.jpg", 0), imread("Shapes2.jpg", 0), imread("Shapes3.jpg", 0), imread("Shapes4.jpg", 0), imread("Shapes5.jpg", 0) };
Mat result = imread("");
Mat translatedImage;
Mat crop;

Rect drawnShape;
Rect roi;

Point centerOfImage(320, 180);
Point center;

int iLastX = -1;
int iLastY = -1;
int posX;
int posY;
int templateWidth = 210;
int templateHeight = 238;
int percentage;
int matchingValue = 0;
int handClosed = 0;
int shape1 = 0;
int shape2 = 0;
int shape3 = 0;
int shape4 = 0;
int shape5 = 0;

Moments oMoments;

float dX;
float dY;

double dM01;
double dM10;
double dArea;
double area;

void drawLine(double contourArea);
void translateImage(Mat input);
void scaleImage(Mat input, float x, float y);
int match(Mat input, double area);

int main()
{
	for (int i = 0; i < 5; i++) {
		threshold(templateArray[i], templateArray[i], 127, 255, THRESH_BINARY);
	}
	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640); // Size of the camera
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);

	namedWindow("Control", CV_WINDOW_NORMAL); //create a window called "Control"

	int iLowH = 79;
	int iHighH = 168;

	int iLowS = 91;
	int iHighS = 170;

	int iLowV = 0;
	int iHighV = 38;

	//int iLowH = 0;	
	//int iHighH = 179;

	//int iLowS = 0;
	//int iHighS = 255;

	//int iLowV = 0;
	//int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	//Capture a temporary image from the camera
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	imgLines = Mat::zeros(imgTmp.rows, imgTmp.cols, CV_8UC3);
	translatedImage = Mat::zeros(imgLines.rows, imgLines.cols, CV_8UC1);

	while (true)
	{
		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
		flip(imgOriginal, imgOriginal, 1);
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		GaussianBlur(imgOriginal, imgHSV, Size(11, 11), 0, 0);
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		findContours(imgThresholded, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

		//Calculate the moments of the thresholded image
		oMoments = moments(imgThresholded);
		dM01 = oMoments.m01;
		dM10 = oMoments.m10;
		dArea = oMoments.m00;

		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());

		for (size_t i = 0; i < contours.size(); i++)
		{
			area = contourArea(contours[i], false);
			drawLine(area);
			//cout << "Area : " << area << "\n";
		}

		imshow("Thresholded Image", imgThresholded); //show the thresholded image

		imgOriginal = imgOriginal + imgLines;
		imshow("Line", imgLines);
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;

}

void drawLine(double contourArea) {
	if (contourArea > 3000 && contourArea < 14000) {
		handClosed = 1;
		posX = (int)(dM10 / dArea);
		posY = (int)(dM01 / dArea);

		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		{
			//Draw a line from the previous point to the current point
			line(imgLines, Point(iLastX, iLastY), Point(posX, posY), Scalar(255, 255, 255), 12);
		}

		iLastX = posX;
		iLastY = posY;
	}

	if (contourArea > 20000 && contourArea < 30000 && handClosed == 1) {
		handClosed = 0;
		iLastX = -1;
		iLastY = -1;

		translateImage(imgLines);
		scaleImage(translatedImage, dX, dY);
		match(crop, area);
		cout << "The returned value is " << shape1 << "\n" << shape2 << "\n" << shape3 << "\n" << shape4 << "\n" << shape5 << endl;
		//imshow("Drawn Shape", imgLines);
		imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);
		translatedImage = Mat::zeros(imgLines.rows, imgLines.cols, CV_8UC1);
	}
}

void translateImage(Mat input) {
	cvtColor(input, input, CV_BGR2GRAY);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(input, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i], false);
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	for (size_t i = 0; i < contours.size(); i++)
	{
		Rect drawnShape(boundRect[i].tl(), boundRect[i].br());
		center = (drawnShape.br() + drawnShape.tl())*0.5;
		dX = (float)templateWidth / drawnShape.width;
		dY = (float)templateHeight / drawnShape.height;
	}

	if (area < 3000) {
		input = Mat::zeros(imgTmp.size(), CV_8UC3);
	}
	else {
		int deltaX = centerOfImage.x - center.x;
		int deltaY = centerOfImage.y - center.y;

		for (int y = 0; y < input.rows; y++) {
			for (int x = 0; x < input.cols; x++) {
				int newX = x + deltaX;
				int newY = y + deltaY;

				if (input.at<uchar>(y, x) == 255) {
					translatedImage.at<uchar>(newY, newX) = input.at<uchar>(y, x);
				}
			}
		}
	}
	//imshow("translatedImage", translatedImage);
}

void scaleImage(Mat input, float x, float y) {
	resize(input, input, Size(), x, y);
	//imshow("scaled", input);
	roi.x = input.cols / 2 - 105;
	roi.y = input.rows / 2 - 119;
	roi.width = 210;
	roi.height = 238;
	crop = input(roi);
	//imshow("cropped", crop);
}

int match(Mat input, double area) {
	if (area > 4000) {
		compare(input, templateArray[0], result, CMP_NE);
		percentage = countNonZero(result);
		imshow("result", result);
		cout << "..........................." << percentage << endl;
		if (percentage < 5000) {
			shape1 = 1;
			shape2 = 0;
			shape3 = 0;
			shape4 = 0;
			shape5 = 0;

			cout << percentage << endl;
			cout << "kill shape 1" << endl;
			imshow("result", result);

			//matchingValue = 1;
			return shape1;
		}
		else if (percentage > 5000) {
			compare(input, templateArray[1], result, CMP_NE);
			percentage = countNonZero(result);
			if (percentage < 5000) {
				shape1 = 0;
				shape2 = 1;
				shape3 = 0;
				shape4 = 0;
				shape5 = 0;
				cout << percentage << endl;
				cout << "kill shape 2" << endl;
				imshow("result", result);

				//matchingValue = 2;
				return shape2;
			}
			else if (percentage > 5000) {
				compare(input, templateArray[2], result, CMP_NE);
				percentage = countNonZero(result);
				if (percentage < 6500) {
					shape1 = 0;
					shape2 = 0;
					shape3 = 1;
					shape4 = 0;
					shape5 = 0;
					cout << percentage << endl;
					cout << "kill shape 3" << endl;
					imshow("result", result);

					//matchingValue = 3;
					return shape3;
				}
				else if (percentage > 5000) {
					compare(input, templateArray[3], result, CMP_NE);
					percentage = countNonZero(result);
					if (percentage < 6500) {
						shape1 = 0;
						shape2 = 0;
						shape3 = 0;
						shape4 = 1;
						shape5 = 0;
						cout << percentage << endl;
						cout << "kill shape 4" << endl;
						imshow("result", result);

						//matchingValue = 4;
						return shape4;
					}
					else if (percentage > 5000) {
						compare(input, templateArray[4], result, CMP_NE);
						percentage = countNonZero(result);
						if (percentage < 10000) {
							shape1 = 0;
							shape2 = 0;
							shape3 = 0;
							shape4 = 0;
							shape5 = 1;
							cout << percentage << endl;
							cout << "kill shape 5" << endl;
							imshow("result", result);

							//matchingValue = 5;
							return shape5;
						}
						else {
							shape1 = 0;
							shape2 = 0;
							shape3 = 0;
							shape4 = 0;
							shape5 = 0;
						}
					}
				}
			}
		}
	}
	else {
		shape1 = 0;
		shape2 = 0;
		shape3 = 0;
		shape4 = 0;
		shape5 = 0;
	}
}