#include <SFML/Graphics.hpp>
#include "src/Board.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "PuzzleToSurvive");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	
	Board board(&window);
	board.setPosition(sf::Vector2f(250,250));


	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::Time elapsedTime = clock.getElapsedTime();
		if (elapsedTime.asMilliseconds() > 1000.0f / 60.0f)
		{

			window.clear();
			//window.draw(shape);
			board.update();
			board.render();
			window.display();
			clock.restart();
		}
	}

	return 0;
}