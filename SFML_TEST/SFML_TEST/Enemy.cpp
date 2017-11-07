#include "Enemy.h"
#include <iostream>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace std;
using namespace cv;

Mat img;

Enemy::Enemy(){}

Enemy::Enemy(float enX, float enY):
	enemyXPostition(enX),
	enemyYPosition(enY){}

Mat Enemy::imageRowCol() {
	img = imread("Dragon.png");
	return img;
}

float Enemy::getEnemyX() {
	cout << "The X position is \n" << enemyXPostition << endl;
	return enemyXPostition;
}

float Enemy::getEnemyY() {
	cout << "The Y position is \n" << enemyYPosition << "\n" << endl;
	return enemyYPosition;
}

void Enemy::showEnemy(Mat frame) {
	////img = imread("Dragon.png");
	////frame = Mat::zeros(img.rows, img.cols, 0);

	//for (int x = 0; x < frame.cols; x++) {
	//	for (int y = 0; y < frame.rows; y++) {
	//		frame.at<unsigned char>(enemyYPosition, enemyXPostition) = 255;

	//	}
	//}

	//putText(frame, "Hi", Point(enemyXPostition, enemyYPosition), FONT_HERSHEY_COMPLEX, 2, Scalar(255, 255, 255), 2, LINE_AA, false);

	//cout << "X spawn position is  " << enemyXPostition << "\nY spawn position is " << enemyYPosition << "\n" << endl;

	//namedWindow("Screen Image", WINDOW_NORMAL);
	//setWindowProperty("Screen Image", CV_WND_PROP_FULLSCREEN, 0);
	//imshow("Screen Image", frame);

	//waitKey(0);
}

void Enemy::deleteEnemy(Mat frame) {
	
}


Enemy::~Enemy(){}
