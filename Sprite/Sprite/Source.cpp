
#include "SFML/Graphics.hpp"
#include <iostream>

using namespace std;



int main(int argc, char** argv)
{
	sf::RenderWindow renderWindow(sf::VideoMode(1000, 715), "Animation");
	sf::Event event;

	sf::Texture texture;
	texture.loadFromFile("monster.png");

	sf::IntRect rectSourceSprite(0, 0, 662, 715);
	sf::Sprite sprite(texture, rectSourceSprite);

	sf::Clock clock;

	//checks if sprite is empty
	if (!texture.loadFromFile("monster.png"))
	{
		std::cout << "Load failed" << std::endl;
		system("pause");
	}
	
	//Handels event
	while (renderWindow.isOpen()) {
		while (renderWindow.pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed)
				renderWindow.close();
		}
		
		//Checks every second wether the rect is on the last frame, of not it will increment
		if (clock.getElapsedTime().asSeconds() > 0.3f) {
			if (rectSourceSprite.left == 6000) {
				rectSourceSprite.left = 0;
			}
			else {
				rectSourceSprite.left += 662;
			}

			//Updating the texture rect 
			sprite.setTextureRect(rectSourceSprite);

			//Starts clock over
			clock.restart();
		}

		renderWindow.clear();
		renderWindow.draw(sprite);
		renderWindow.display();

	}


	return EXIT_SUCCESS;
}


/*

#include <iostream>

#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(6620, 715), "Awesome Game");

	sf::Texture texture;

	//texture.setRepeated(true);

	if (!texture.loadFromFile("monster.png"))
	{
		std::cout << "Load failed" << std::endl;
		

		system("pause");
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);

//	sprite.setTextureRect(sf::IntRect(0, 0, 600, 600));

	//sprite.setColor(sf::Color(255, 255, 255, 64));

	while (window.isOpen())
	{
		sf::Event event;

		// handle all events
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();

				break;
			}
		}

		// update the game

		window.clear();

		window.draw(sprite);

		window.display();
	}

	return EXIT_SUCCESS;
}
*/
