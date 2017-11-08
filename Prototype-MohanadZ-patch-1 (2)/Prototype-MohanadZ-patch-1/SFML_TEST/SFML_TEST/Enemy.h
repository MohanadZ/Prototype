#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#ifndef ENEMY_H

using namespace cv;

class Enemy
{
private:
	int enemyXPostition;
	int enemyYPosition;

public:
	Enemy();
	Enemy(float enemyXPosition, float enemyYPosition);
	Mat imageRowCol();
	float getEnemyX();
	float getEnemyY();
	void showEnemy(Mat frame);
	void deleteEnemy(Mat frame);

	~Enemy();
};
#endif // !ENEMY_H



