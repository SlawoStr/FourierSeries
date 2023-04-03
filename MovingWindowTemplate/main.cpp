#include "MovingWindow.h"
#include <iostream>

int main()
{
	MovingWindow window(sf::Vector2u(1920, 1080),"Simulation",sf::Style::Fullscreen,144);

	window.run();

	return 0;
}