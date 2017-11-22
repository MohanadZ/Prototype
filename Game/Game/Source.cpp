#include "SFML/Graphics.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "Avatar.h"
#include "Monster.h"
#include "SpriteAnimation.h"

using namespace std;
using namespace sf;

int idle = 1, attack, dying;

Event windowEvt;		//Create an Event to be able to interact with the window; close, resize, etc.
time_t sec;

int gameW = 1920, gameH = 1080;		//The size of the game window
const int number = 7;
int position, axis, side;

int score = 0;

float difficulty = 0.0004f;
int monsterNumber = 1;

Clock spriteClock;

Texture bgr;
Texture hp0, hp1, hp2, hp3;
Texture decoyAvatarTexture;

Sprite bgrSprite;
Sprite hpSprite;

Font font;
Text scoreText;

Vector2u idleMonsterTextureSize;

Monster monster[number];

float deltaTime;
int a_idle = 1, a_attack, a_damaged, a_dying;

void separateMonsters(int i);
void setDifficulty();
void updateScore(int s);

int main(int argc, char** argv) {
	time(&sec);
	srand((unsigned char)sec);

	//Create a window for the game
	RenderWindow gameWindow(VideoMode((unsigned int)gameW, (unsigned int)gameH), "Game", Style::Default);

	//Background 
	bgr.loadFromFile("Background.png");
	bgrSprite.setTexture(bgr);

	//Avatar HP
	hp0.loadFromFile("Hearts0.png");
	hp1.loadFromFile("Hearts1.png");
	hp2.loadFromFile("Hearts2.png");
	hp3.loadFromFile("Hearts3.png");

	updateScore(score);

	//Avatar
	Avatar wizard = Avatar();

	wizard.createAvatar(1);

	//IntRect avatarRectSourceSprite(0, 0, wizard.avatarTextureSize.x, wizard.avatarTextureSize.y);

	//decoyAvatarTexture.loadFromFile("WizardIdle.png");

	//Monster
	for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {
		monster[i].createMonsters();
		separateMonsters(i);

		monster[i].monsterSprite.setPosition((float)(monster[i].monsterX), (float)(monster[i].monsterY));

		monster[i].monstersSpeed(wizard.avatarSprite.getPosition().x, wizard.avatarSprite.getPosition().y, difficulty);
	}

	//idleMonsterTextureSize = monster[0].monsterTexture.getSize();
	//idleMonsterTextureSize.x /= 20;
	//IntRect enemyRectSourceSprite(0, 0, idleMonsterTextureSize.x, idleMonsterTextureSize.y);

	Texture avatarT, monsterT, monsterRT;
	avatarT.loadFromFile("WizardSheet.png");
	monsterT.loadFromFile("MonsterSheet.png");

	SpriteAnimation avatarAnimation(&avatarT, Vector2u(20, 4), 0.05f);

	for (int x = 0; x < number; x++) {
		monster[x].anim = { &monsterT, Vector2u(20, 4), 0.05f };
	}

	int testShape[number];
	for (int i = 0; i < number; i++) {
		testShape[i] = rand() % 300;
		if (testShape[i] <= 60) {
			testShape[i] = 0;
		}
		if (testShape[i] <= 120 && testShape[i] > 60) {
			testShape[i] = 1;
		}
		if (testShape[i] <= 180 && testShape[i] > 120) {
			testShape[i] = 2;
		}
		if (testShape[i] <= 240 && testShape[i] > 180) {
			testShape[i] = 3;
		}
		if (testShape[i] <= 300 && testShape[i] > 240) {
			testShape[i] = 4;
		}
		cout << "................................." << testShape[i] << endl;
	}

	//Start the game loop in order for the window to stay open
	while (gameWindow.isOpen()) {
		while (gameWindow.pollEvent(windowEvt)) {

			//Switch case for handling the events performed when interacting with the game window
			switch (windowEvt.type) {

			case Event::Closed:				//Close the window when Close is clicked
				gameWindow.close();

				break;

			case Event::Resized:			//Print to the console the size of the window when resizing 
				cout << "New game window width = " << windowEvt.size.width << " New game window height = " << windowEvt.size.height << endl;
				break;

			default:
				break;
			}
		}

		deltaTime = spriteClock.restart().asSeconds();

		//Update the animation of the avatar sprite
		if (a_idle == 1) {
			avatarAnimation.updateAnimation(0, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}
		if (a_attack == 1) {
			avatarAnimation.updateAnimation(1, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}
		if (a_damaged == 1) {
			avatarAnimation.updateAnimation(2, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}
		if (a_dying == 1) {
			avatarAnimation.updateAnimation(3, deltaTime);
			wizard.avatarSprite.setTextureRect(avatarAnimation.textureRect);
		}

		//Update the animation of the monster sprite
		for (int z = 0; z < number; z++) {
			if (monster[z].idle == 1) {
				monster[z].anim.updateAnimation(testShape[z], deltaTime);
					monster[z].monsterSprite.setTextureRect(monster[z].anim.textureRect);
			}
			if (monster[z].attack == 1) {
				monster[z].anim.updateAnimation(2, deltaTime);
				monster[z].monsterSprite.setTextureRect(monster[z].anim.textureRect);
			}
			if (monster[z].dying == 1) {
				monster[z].anim.updateAnimation(3, deltaTime);
				monster[z].monsterSprite.setTextureRect(monster[z].anim.textureRect);
			}
		}
		////The animation of the sprites. Checks every second whether the rect is on the last frame, if not it will increment
		//if (spriteClock.getElapsedTime().asSeconds() > 0.05f) {
		//	//Avatar
		//	if (avatarRectSourceSprite.left < 11340) {
		//		avatarRectSourceSprite.left += wizard.avatarTextureSize.x;
		//	}
		//	if (avatarRectSourceSprite.left >= 11340) {
		//		avatarRectSourceSprite.left = 0;
		//		wizard.avatarSprite.setTexture(decoyAvatarTexture);
		//	}

		//	//Monster
		//	if (enemyRectSourceSprite.left < 13240) {
		//		enemyRectSourceSprite.left += idleMonsterTextureSize.x;
		//	}
		//	if (enemyRectSourceSprite.left >= 13240) {
		//		enemyRectSourceSprite.left = 0;
		//	}

		//	wizard.avatarSprite.setTextureRect(avatarRectSourceSprite);

		//	for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {
		//		//Updating the texture rect
		//		monster[i].monsterSprite.setTextureRect(enemyRectSourceSprite);
		//	}

		//	//Starts clock over
		//	spriteClock.restart();
		//}

		for (int i = 0; i < monsterNumber; i++) {
			monster[i].moveMonsters();

			if (monster[i].monsterSprite.getGlobalBounds().intersects(wizard.decoyAvatarSprite.getGlobalBounds())) {
				if (monster[i].idle == 1) {
					monster[i].idle = 0;
					monster[i].attack = 1;
					monster[i].speedX = 0;
					monster[i].speedY = 0;
					monster[i].anim.currentImage.x = 0;
					avatarAnimation.currentImage.x = 0;
				}

				if (monster[i].attack == 1 && monster[i].anim.currentImage.x == 19){ //&& avatarAnimation.currentImage.x == 19) {
					monster[i].dying = 1;
					monster[i].attack = 0;
					a_idle = 0;
					a_damaged = 1;
					monster[i].anim.currentImage.x = 0;
					avatarAnimation.currentImage.x = 0;
				}
			
				if (monster[i].dying == 1 && monster[i].anim.currentImage.x == 19){ //&& avatarAnimation.currentImage.x == 19) {
					monster[i].dying = 0;
					monster[i].idle = 1;
					a_damaged = 0;
					a_idle = 1;
					setDifficulty();

					separateMonsters(i);

					monster[i].monsterSprite.setPosition((float)(monster[i].monsterX), (float)(monster[i].monsterY));

					testShape[i] = rand() % 300;
					
					//for (int i = 0; i < number; i++) {
						testShape[i] = rand() % 300;
						if (testShape[i] <= 60) {
							testShape[i] = 0;
						}
						if (testShape[i] <= 120 && testShape[i] > 60) {
							testShape[i] = 1;
						}
						if (testShape[i] <= 180 && testShape[i] > 120) {
							testShape[i] = 2;
						}
						if (testShape[i] <= 240 && testShape[i] > 180) {
							testShape[i] = 3;
						}
						if (testShape[i] <= 300 && testShape[i] > 240) {
							testShape[i] = 4;
						}
						cout << "Another spawn random____________________ " << testShape[i] << endl;
					//}
				
					monster[i].anim.currentImage.y = testShape[i];

					monster[i].monstersSpeed(wizard.avatarSprite.getPosition().x, wizard.avatarSprite.getPosition().y, difficulty);
					monster[i].moveMonsters();

					
					//avatarRectSourceSprite.left = 0;
					//wizard.createAvatar(0);

					score += 100;
					updateScore(score);

					wizard.avatarLife--;
				}

			}
		}

		//cout << s << endl;

		//cout << "The avatar's hp is " << wizard.avatarLife << endl;
		if (wizard.avatarLife == 3) {
			hpSprite.setTexture(hp3);
		}
		else if (wizard.avatarLife == 2) {
			hpSprite.setTexture(hp2);
		}
		else if (wizard.avatarLife == 1) {
			hpSprite.setTexture(hp1);
		}
		else if (wizard.avatarLife == 0) {
			hpSprite.setTexture(hp0);
		}
		if (wizard.avatarLife < 0) {
			wizard.avatarLife = 0;
		}

		gameWindow.clear();

		gameWindow.draw(bgrSprite);
		gameWindow.draw(hpSprite);
		gameWindow.draw(scoreText);
		gameWindow.draw(wizard.avatarSprite);
		//gameWindow.draw(damagedWizard.avatarSprite);
		for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {
			gameWindow.draw(monster[i].monsterSprite);
		}
		gameWindow.display();
	}

	return 0;
}

//Spawn the monsters without them overlapping 
void separateMonsters(int i) {
	int x = 0, v = 0;

	idleMonsterTextureSize = monster[0].monsterTexture.getSize();
	idleMonsterTextureSize.x /= 20;
	idleMonsterTextureSize.y /= 4;

	while (i < sizeof(monster) / sizeof(monster[0])) {
		//axis = 0, side = 0	top edge
		//axis = 0, side = 1	bottom edge
		//axis = 1, side = 0	left edge
		//axis = 1, side = 1	right edge

		axis = rand() % 120;
		side = rand() % 120;

		if (axis <= 60) {
			axis = 0;
		}
		else {
			axis = 1;
		}

		if (side <= 60) {
			side = 0;
		}
		else {
			side = 1;
		}

		if (axis == 0) {
			position = rand() % gameW;
		}
		else {
			position = rand() % gameH;
		}

		if (side == 0 && axis == 0) {
			monster[i].monsterX = (float)position;
			monster[i].monsterY = -140;
		}
		else if (side == 1 && axis == 0) {
			monster[i].monsterX = (float)position;
			monster[i].monsterY = (float)gameH + 51;
		}
		else if (side == 0 && axis == 1) {
			monster[i].monsterX = -140;
			monster[i].monsterY = (float)position;
		}
		else if (side == 1 && axis == 1) {
			monster[i].monsterX = (float)gameW + 51;
			monster[i].monsterY = (float)position;
		}

		if (i >= 1) {
			//x-axis
			for (int number = 0; number < i; number++) {
				if (v <= 5) {
					if (monster[i].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 > monster[number].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2 && monster[i].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 < monster[number].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 || monster[i].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2 < monster[number].monsterX + (idleMonsterTextureSize.x * monster[i].scaleW) / 2 && monster[i].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2 > monster[number].monsterX - (idleMonsterTextureSize.x * monster[i].scaleW) / 2) {
						v++;
						separateMonsters(i);
						x++;
					}
					v = 0;
				}
			}

			//y-axis
			for (int number = 0; number < i; number++) {
				if (v <= 5) {
					if (monster[i].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 > monster[number].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2 && monster[i].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 < monster[number].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 || monster[i].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2 < monster[number].monsterY + (idleMonsterTextureSize.y * monster[i].scaleH) / 2 && monster[i].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2 > monster[number].monsterY - (idleMonsterTextureSize.y * monster[i].scaleH) / 2) {
						v++;
						separateMonsters(i);
						x++;
					}
					v = 0;
				}
			}
		}
		i++;
	}
	cout << "The separate is running " << x << " times" << endl;
}

void setDifficulty() {
	difficulty += 0.000005f;
	if (difficulty >= 0.00041) {
		monsterNumber = 2;
	}
	if (difficulty >= 0.00045) {
		monsterNumber = 3;
	}
	if (difficulty >= 0.00051) {
		monsterNumber = 4;
	}
	if (difficulty >= 0.00062) {
		monsterNumber = 5;
	}
	if (difficulty >= 0.00073) {
		monsterNumber = 6;
	}
	if (difficulty >= 0.00085) {
		monsterNumber = 7;
	}
}

void updateScore(int s) {
	//Font & Text
	String currentScore = "SCORE  " + to_string(s);

	font.loadFromFile("Pixeled.ttf");
	scoreText = Text(currentScore, font, 24);
	scoreText.setFillColor(Color::Yellow);
	scoreText.setStyle(Text::Italic | Text::Bold);
	scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2);
	scoreText.setPosition((float)gameW - 230, (float)30);
}