#include "SpriteAnimation.h"
#include <iostream>

using namespace std;


SpriteAnimation::SpriteAnimation(Texture* texture, Vector2u imageCount, float switchImageTime) :
	imageCount(imageCount),
	switchImageTime(switchImageTime)
{
	totalTime = 0.0f;
	currentImage.x = 0;

	//width and height
	textureRect.width = (int) (texture->getSize().x / imageCount.x);
	textureRect.height = (int) (texture->getSize().y / imageCount.y);

	//cout << "The width monster texture is " << (int) (texture->getSize().x) << endl;
	//cout << "The width of the single monster is " << (int) (texture->getSize().x / imageCount.x) << endl;
}

void SpriteAnimation::updateAnimation(int row, float deltaTime)
{
	currentImage.y = row;
	totalTime += deltaTime;

	if (totalTime >= switchImageTime) {
		totalTime -= switchImageTime;
		currentImage.x++;

		if (currentImage.x >= imageCount.x) {
			currentImage.x = 0;
		}
	}

	textureRect.left = currentImage.x * textureRect.width;
	textureRect.top = currentImage.y * textureRect.height;
}


SpriteAnimation::~SpriteAnimation()
{
}
