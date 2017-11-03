#pragma once
#ifndef ENEMY_H

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
	int getEnemyX();
	int getEnemyY();
	void showEnemy();
};

#endif // !ENEMY_H

