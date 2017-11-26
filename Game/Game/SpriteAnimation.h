#pragma once
#include "SFML\Graphics.hpp"

using namespace sf;

#ifndef SPRITEANIMATION_H

class SpriteAnimation
{
private: 
	Vector2u imageCount;

	float totalTime;
	float switchImageTime;

public:
	IntRect textureRect;
	Vector2u currentImage;

	SpriteAnimation();
	SpriteAnimation(Texture* texture, Vector2u imageCount, float switchImageTime);
	void updateAnimation(int row, float deltaTime);		//deltaTime is the time between the previous frame and the current one

	~SpriteAnimation();
};

#endif // !SPRITEANIMATION_H




