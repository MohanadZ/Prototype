#include <iostream>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "RandomEnemy.h"

using namespace std;
using namespace cv;

RandomEnemy::RandomEnemy() {}

RandomEnemy::RandomEnemy(int enX, int enY):
	enemyXPostition(enX),
	enemyYPosition(enY){}

//void RandomEnemy::setEnemyX(int newXPos) {
//	enemyXPostition = newXPos;
//}
//
//void RandomEnemy::setEnemyY(int newYPos) {
//	enemyYPosition = newYPos;
//}

int RandomEnemy::getEnemyX() {
	cout << "The X position is \n" << enemyXPostition << endl;
	return enemyXPostition;
}

int RandomEnemy::getEnemyY(){
	cout << "The Y position is \n" << enemyYPosition << "\n" << endl;
	return enemyYPosition;
}

void RandomEnemy::showEnemy() {
	Mat img, frame;
	img = imread("Dragon.png");
	frame = Mat::zeros(img.rows, img.cols, 0);

	for (int x = 0; x < frame.cols; x++) {
		for (int y = 0; y < frame.rows; y++) {
			frame.at<unsigned char>(enemyYPosition, enemyXPostition) = 255;
		}
	}
	cout << "............................ " << enemyXPostition << "\n" << enemyYPosition << endl;

	imshow("Screen Image", frame);

	waitKey(0);
}


/*
for (int x = 0; x < img.cols; x++) {

imgLine = Mat::zeros(imgLine.rows, imgLine.cols, CV_8UC3);
line(imgLine, Point(30 + x, 50), Point(70 + x, 70), CV_RGB(0, 0, 255), 6);
//disp = zeros(disp.rows, disp.cols);
}
*/