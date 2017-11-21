#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

#ifndef AVATAR_H

class Avatar
{
private:
	int gameW = 1920, gameH = 1080;	
	//int avatarLabel = 1;	//1 means the avatar is idle
	float avatarX, avatarY;
	float avatarW = 0.75f, avatarH = 0.75f;

public:
	int avatarLife = 3;
	Sprite avatarSprite, decoyAvatarSprite, damagedAvatarSprite;
	Texture avatarTexture;
	Vector2u avatarTextureSize;

	Avatar();
	void createAvatar(int avatarLabel);
	~Avatar();
};

#endif // !AVATAR_H

