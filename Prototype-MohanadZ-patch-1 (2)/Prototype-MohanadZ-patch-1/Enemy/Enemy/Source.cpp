#include <stdlib.h>  
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include "RandomEnemy.h"

using namespace std;

time_t seconds;
int objX, objY;

Mat img, frame;

int main() {
	/*time_t test;

	time(&test);

	srand((unsigned char)test);

	cout << "THE RANDOM NUMBER IS " << rand() % 200 << endl;
	cout << ".................. " << rand() % 10 << endl;
	system("pause");*/

	img = imread("Dragon.png");
	frame = Mat::zeros(img.rows, img.cols, 0);

	time(&seconds);
	srand((unsigned char)seconds);

	RandomEnemy obj[10];
	for (int i = 0; i < 10; i++) {
		int axis = rand() % 120;
		int side = rand() % 120;
		int pos;
		
		if (axis <= 60) {
			axis = 0;
		}
		else {
			axis = 1;
		}
		if (side <= 60) {
			side = 0;
		}
		else {
			side = 1;
		}
		
		if (axis == 0) {
			pos = rand() % img.cols;
		}
		else {
			pos = rand() % img.rows;
		}
		if (side == false && axis == false) {
			objX = pos;
			objY = 0;
		}
		else if (side == true && axis == false) {
			objX = pos;
			objY = img.rows - 1;
		}
		else if (side == false && axis == true) {
			objX = 0;
			objY = pos;
		}
		else if (side == true && axis == true) {
			objX = img.cols - 1;
			objY = pos;
		}

		//Add if-statement to exclude the range at which the avatar is going to be placed at
		cout << axis << endl;
		cout << side << endl;
		obj[i] = RandomEnemy(objX, objY);
		obj[i].getEnemyX();
		obj[i].getEnemyY();
		obj[i].showEnemy(frame);
	}
	system("pause");

	return 0;
}
