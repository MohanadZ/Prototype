#include <stdlib.h>  
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include "RandomEnemy.h"

using namespace std;

time_t seconds;

int main() {
	/*time_t test;

	time(&test);

	srand((unsigned char)test);

	cout << "THE RANDOM NUMBER IS " << rand() % 200 << endl;
	cout << ".................. " << rand() % 10 << endl;
	system("pause");*/

	time(&seconds);
	srand((unsigned char)seconds);
	
	RandomEnemy obj[10];
	for (int i = 0; i < 10; i++) {
		int objX = rand() % obj[i].imageRowCol().cols;
		int objY = rand() % obj[i].imageRowCol().rows;

		//Add if-statement to exclude the range at which the avatar is going to be placed at

		obj[i] = RandomEnemy(objX, objY);
		obj[i].getEnemyX();
		obj[i].getEnemyY();
		obj[i].showEnemy();
	}
	system("pause");

	return 0;
}




/*
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

int main(int args, char** argv) {
Mat img;
Mat imgLine;

img = imread("Dragon.png", 1);
imgLine = img.clone();

imshow("Input", img);

for (int x = 0; x < img.cols; x++) {

imgLine = Mat::zeros(imgLine.rows, imgLine.cols, CV_8UC3);
line(imgLine, Point(30 + x, 50), Point(70 + x, 70), CV_RGB(0, 0, 255), 6);
//disp = zeros(disp.rows, disp.cols);
}

//int x = 0;
//
//while (x < 300) {
//imgline = mat::zeros(imgline.rows, imgline.cols, cv_8uc3);
//line(imgline, point(30 + x, 50), point(70 + x, 70), cv_rgb(0, 0, 255), 6);
//x++;
//}

imshow("Output", imgLine);
waitKey(0);
}
*/