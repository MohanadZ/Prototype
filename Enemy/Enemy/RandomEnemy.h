#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#ifndef ENEMY_H

using namespace cv;

class RandomEnemy
{
private:
	int enemyXPostition;
	int enemyYPosition;

public:
	//constructor
	RandomEnemy();
	RandomEnemy(int enemyXPosition, int enemyYPosition);
	//void setEnemyX(int newXPos);
	//void setEnemyY(int newYPos);
	Mat imageRowCol();
	int getEnemyX();
	int getEnemyY();
	void showEnemy();
};

#endif // !ENEMY_H

