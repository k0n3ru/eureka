#include <iostream>
#include <shapes.h>
#include <SFML/Window.hpp>

int main()
{
	std::cout<<"Hello World"<<std::endl;
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	window.setFramerateLimit(60);

	Eureka::Tetromino block_1(sf::Vector2f(100,100),Eureka::SHAPE_I);
	Eureka::Tetromino block_2(sf::Vector2f(180,100),Eureka::SHAPE_J);
	Eureka::Tetromino block_3(sf::Vector2f(240,100),Eureka::SHAPE_L);
	Eureka::Tetromino block_4(sf::Vector2f(320,100),Eureka::SHAPE_Z);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		window.draw(block_1);
		window.draw(block_2);
		window.draw(block_3);
		window.draw(block_4);

		window.display();
	}

	return 0;
}
