#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

#ifndef AVATAR_H

class Avatar
{
private:
	int gameW = 1920, gameH = 1080;	
	float avatarX, avatarY;
	float avatarW = 0.8f, avatarH = 0.8f;

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

