#include "Monster.h"
#include "Avatar.h"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace sf;
using namespace std;

Avatar m_avatar = Avatar();

Monster::Monster() {}

void Monster::loadTexture(int label) {
	if (label == 0) {
		monsterTexture.loadFromFile("MonsterAll.png");
	}
	monsterSprite.setTexture(monsterTexture);
	monsterSprite.setScale(scaleW, scaleH);
}

void Monster::createMonsters() {
	loadTexture(0);
}

//calculate the speed of the sprites towards the avatar
void Monster::monstersSpeed(float avatarPosX, float avatarPosY, float difficulty) {

	speedX = (avatarPosX - monsterSprite.getPosition().x) * difficulty;
	speedY = (avatarPosY - monsterSprite.getPosition().y) * difficulty;

}

//Moving the enemy sprites
void Monster::moveMonsters() {
	monsterSprite.move(speedX, speedY);
}

//bool Monster::detectCollision() {
//	//m_avatar.createAvatar();
//
//	if (monsterSprite.getGlobalBounds().intersects(m_avatar.decoyAvatarSprite.getGlobalBounds())) {
//
//		////time_t srand(time(NULL));
//		//monsterX = (float) (rand() % 120);
//		//monsterY = (float) (rand() % 120);
//
//		//monsterSprite.setPosition(monsterX, monsterY);
//		//monstersSpeed(monsterX, monsterY);
//		//moveMonsters();
//		return true;
//	}
//	else
//
//		return false;
//}

Monster::~Monster()
{
}