#include "SFML/Graphics.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace sf;

Event windowEvt;		//Create an Event to be able to interact with the window; close, resize, etc.
time_t sec;

float objXArray[20];
float objYArray[20];

int main(int argc, char** argv) {

	time(&sec);
	srand((unsigned char)sec);

	//Create a window for the game
	float gameW = 1920, gameH = 1080;
	RenderWindow gameWindow(VideoMode((unsigned int) gameW, (unsigned int) gameH), "Game", Style::Default);

	//Create an avatar
	RectangleShape avatar(Vector2f(100.0f, 100.0f));		//Vec2f contains an x and a y coordinate of type float
	avatar.setPosition(gameW / 2, gameH / 2);				//Set the position of the avatar

	//Create monsters
	RectangleShape monster[20];
	for (int i = 0; i < sizeof(monster) / sizeof(monster[0]); i++) {			//sizeof(array) returns the total number of bytes allocated for that array. To get the number of elements in the array, divide by the size of one element in the array sizeof(array[0] 

		objXArray[i] = (float) (rand() % 1920);
		objYArray[i] = (float) (rand() % 1080);

		if (objXArray[i] >= (gameW / 2 - 150) && objXArray[i] < (gameW / 2 + 150) && objYArray[i] >= (gameW / 2 - 150) && objYArray[i] < (gameW / 2 + 150)) {
			objXArray[i] = (float) (rand() % 1920);
			objYArray[i] = (float) (rand() % 1080);
		}

		monster[i].setSize(Vector2f(50.0f, 50.0f));
		monster[i].setPosition(objXArray[i], objYArray[i]);
		monster[i].setFillColor(Color::Cyan);

		cout << "The X position is \n" << objXArray[i] << endl;
		cout << "The Y position is \n" << objYArray[i] << "\n" << endl;
	}

	//while loop to create the game loop in order for the window to stay open
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

		for (int j = 0; j < sizeof(monster) / sizeof(monster[0]); j++) {
			if (objXArray[j] >= gameW / 2 && objYArray[j] < gameH / 2) {
				monster[j].move(-0.1f, 0.1f);
			}
			if (objXArray[j] < gameW / 2 && objYArray[j] < gameH / 2) {
				monster[j].move(0.1f, 0.1f);
			}
			if (objXArray[j] >= gameW / 2 && objYArray[j] >= gameH / 2) {
				monster[j].move(-0.1f, -0.1f);
			}
			if (objXArray[j] < gameW / 2 && objYArray[j] >= gameH / 2) {
				monster[j].move(0.1f, -0.1f);
			}
		}
		
		gameWindow.clear();
		//Draw the avatar and display it
		gameWindow.draw(avatar);
		for (int i = 0; i < 20; i++) {
			gameWindow.draw(monster[i]);
		}
		gameWindow.display();
	}

	return 0;
}