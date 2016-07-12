#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main()
{
	std::cout<<"Hello World"<<std::endl;
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);
		window.display();
	}

	return 0;
}
