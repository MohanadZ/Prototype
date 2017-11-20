#include "Avatar.h"
#include <iostream>

using namespace sf;
using namespace std;

Avatar::Avatar() {}

void Avatar::createAvatar(int avatarLabel) {
	if (avatarLabel == 1) {
		avatarTexture.loadFromFile("WizardIdle.png");
	}

	if (avatarLabel == 0) {
		avatarTexture.loadFromFile("WizardDamaged.png");
	}

	avatarSprite.setTexture(avatarTexture);
	decoyAvatarSprite.setTexture(avatarTexture);

	avatarTextureSize = avatarTexture.getSize();
	avatarTextureSize.x /= 20;

	avatarX = (float) avatarTextureSize.x;
	avatarY = (float) avatarTextureSize.y;

	avatarSprite.setScale(avatarW, avatarH);
	decoyAvatarSprite.setScale(avatarW / 20, avatarH);

	avatarSprite.setPosition((float)(gameW / 2) - (avatarX * avatarW) / 2, (float)(gameH / 2) - (avatarY * avatarH) / 2);
	decoyAvatarSprite.setPosition((float)(gameW / 2) - (avatarX * avatarW) / 2, (float)(gameH / 2) - (avatarY * avatarH) / 2);

	cout << "................... >:( " << endl;
}

Avatar::~Avatar()
{
}
