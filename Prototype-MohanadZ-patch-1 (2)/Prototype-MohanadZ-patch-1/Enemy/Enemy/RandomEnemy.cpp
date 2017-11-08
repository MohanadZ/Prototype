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

Mat RandomEnemy::imageRowCol() {
	Mat img = imread("Dragon.png");
	return img;
}

int RandomEnemy::getEnemyX() {
	cout << "The X position is \n" << enemyXPostition << endl;
	return enemyXPostition;
}

int RandomEnemy::getEnemyY(){
	cout << "The Y position is \n" << enemyYPosition << "\n" << endl;
	return enemyYPosition;
}

void RandomEnemy::showEnemy(Mat frame) {
	for (int x = 0; x < frame.cols; x++) {
		for (int y = 0; y < frame.rows; y++) {
			frame.at<unsigned char>(enemyYPosition, enemyXPostition) = 255;
		}
	}
	cout << "X spawn position is  " << enemyXPostition << "\nY spawn position is " << enemyYPosition << "\n" << endl;

	namedWindow("Screen Image", WINDOW_NORMAL);
	setWindowProperty("Screen Image", CV_WND_PROP_FULLSCREEN, 0);
	imshow("Screen Image", frame);

	waitKey(0);
}