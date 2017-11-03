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
