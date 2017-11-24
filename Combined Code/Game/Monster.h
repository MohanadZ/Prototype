#pragma once
#include "SFML/Graphics.hpp"
#include "SpriteAnimation.h"

using namespace sf;

#ifndef MONSTER_H

class Monster
{
public:
	Sprite monsterSprite;
	Texture monsterTexture;
	SpriteAnimation anim {&monsterTexture, Vector2u(20,4), 0.05f};
	int monsterLabel;
	float monsterX, monsterY;
	int width = 567, height = 874;
	float scaleW = 0.50f, scaleH = 0.50f;
	float speedX, speedY;
	int idle = 1, attack, dying;

	Monster();
	void loadTexture(int label);
	void createMonsters();
	void monstersSpeed(float x, float y, float difficulty);
	void moveMonsters();
	//bool detectCollision();

	~Monster();
};

#endif // !MONSTER_H