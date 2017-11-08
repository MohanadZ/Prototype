#include "SFML/Graphics.hpp"
#include <iostream>

using namespace std;
using namespace sf;

int main(int argc, char** argv)
{
	//Create a window for the game
	float gameW = 1920, gameH = 1080;
	RenderWindow gameWindow(VideoMode(gameW, gameH), "Game", Style::Default);

	//Create an avatar
	RectangleShape avatar(Vector2f(100.0f, 100.0f));		//Vec2f contains an x and a y coordinate of type float
	avatar.setPosition(gameW / 2, gameH / 2);				//Set the position of the avatar

	//while loop to create the game loop in order for the window to stay open
	while (gameWindow.isOpen()) {
		Event windowEvt;		//Create an Event to be able to interact with the window; close, resize, etc.
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

		//Draw the avatar and display it
		gameWindow.draw(avatar);
		gameWindow.display();
	}

	return 0;
}