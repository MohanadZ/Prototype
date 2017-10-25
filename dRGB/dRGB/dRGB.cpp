// This code captures images directly from the webcam

#include "stdafx.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\core\mat.inl.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat frame;
Mat thresholdImage;

int countR = 0, countG = 0, countB = 0;

int bMin = 10;
int bMax = 50;
int gMin = 50;
int gMax = 100;
int rMin = 10;
int rMax = 50;

void detectHand();

int main(int argc, char** argv) {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	while (true) {
		bool access = cap.read(frame);
		thresholdImage = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
		//thresholdImage = frame.clone();

		if (!access) {
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
		imshow("Current image", frame);

		if (waitKey(30) == 27) {
			cout << "ESC key is pressed" << endl;
			break;
		};

		//if (waitKey(30) == 32 && waitKey(30) != 27) {
			detectHand();
		//}
		imshow("Threshold Image", thresholdImage);
	}

	return 0;
}

void detectHand() {

	if (frame.data && !frame.empty()) {
		try {
			for (size_t x = 0; x < frame.cols; x++) {
				for (size_t y = 0; y < frame.rows; y++) {
					//Vec3b pixel = frame.at<Vec3b>(x, y);
					//countB = pixel[0];
					//countG = pixel[1];
					//countR = pixel[2];

					countB = frame.at<Vec3b>(y, x)[0];
					/*if (bMin < countB < bMax) {
						thresholdImage.at<Vec3b>(y, x)[0] = 0;
					}
					else {
						thresholdImage.at<Vec3b>(y, x)[0] = 255;
					}*/

					countG = frame.at<Vec3b>(y, x)[1];
					if (countG > gMax) {
						thresholdImage.at<unsigned char>(y, x) = 255;
					}
					else {
						thresholdImage.at<unsigned char>(y, x) = 0;
					}

					countR = frame.at<Vec3b>(y, x)[2];
					/*if (rMin < countR < rMax) {
						thresholdImage.at<Vec3b>(y, x)[2] = 0;
					}
					else {
						thresholdImage.at<Vec3b>(y, x)[2] = 255;
					}*/
				}
			}
		}
		catch (int e) {
			cout << "An exception occured. Exception is : " << e << endl;
		}

		int sum = frame.cols * frame.rows;
		cout << "The average is: " << countG << endl;
	}
}

