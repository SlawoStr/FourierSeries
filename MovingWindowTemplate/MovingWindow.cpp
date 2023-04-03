#include "MovingWindow.h"

MovingWindow::MovingWindow(sf::Vector2u windowSize, sf::String title, sf::Uint32 style, unsigned int framerate) : window(sf::VideoMode(windowSize.x, windowSize.y), title, style), generator()
{
	// Max Framerate
	window.setFramerateLimit(framerate);
	// Set view to start in the point (0,0)
	sf::View view = window.getView();
	view.move(-static_cast<float>(windowSize.x) / 2, -static_cast<float>(windowSize.x) / 2);
	window.setView(view);
	// Load font
	font.loadFromFile("font.ttf");

	// Controlls
	this->framerate = framerate;
	circleSpeed = 1.0f;
	circleRadius = 500.0f;
}

void MovingWindow::run()
{
	float deltaTime{ 0.0f };
	while (window.isOpen())
	{
		deltaTime += 0.01f;
		window.clear(sf::Color::Black);
		draw();
		window.display();
		update(deltaTime);
		pollEvent();
	}
}

void MovingWindow::pollEvent()
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::Resized:
		{
			sf::View view = window.getView();
			view.setSize(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
			view.zoom(zoomLevel);
			window.setView(view);
			break;
		}
		case sf::Event::MouseMoved:
			if (panning)
			{
				sf::View view = window.getView();
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window) - this->panningAnchor);
				view.move(-1.0f * pos * zoomLevel);
				window.setView(view);
				panningAnchor = sf::Mouse::getPosition(window);
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Middle)
			{
				panning = true;
				panningAnchor = sf::Mouse::getPosition(window);
			}
			else if (e.mouseButton.button == sf::Mouse::Left)
			{
				generator.fullClear();
				generator.addCircle();
			}
			else if (e.mouseButton.button == sf::Mouse::Right)
			{
				generator.fullClear();
				generator.addCircle(circleRadius,circleSpeed);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Middle)
			{
				panning = false;
			}
			break;
		case sf::Event::MouseWheelMoved:
		{
			sf::View view = window.getView();
			if (e.mouseWheel.delta < 0)
			{
				view.zoom(2.0f);
				zoomLevel *= 2;
			}
			else
			{
				view.zoom(0.5f);
				zoomLevel *= 0.5;
			}
			window.setView(view);
			break;
		}
		case sf::Event::KeyPressed:
		{
			switch (e.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;

			case sf::Keyboard::Add:
				framerate += 1;
				window.setFramerateLimit(framerate);
				break;

			case sf::Keyboard::Subtract:
				framerate -= 1;
				window.setFramerateLimit(framerate);
				break;
				
			case sf::Keyboard::W:
				circleSpeed += 0.01f;
				break;

			case sf::Keyboard::S:
				circleSpeed -= 0.01f;
				break;

			case sf::Keyboard::A:
				circleRadius -= 1.0f;
				break;

			case sf::Keyboard::D:
				circleRadius += 1.0f;
				break;

			case sf::Keyboard::Left:
				generator.prevCircle();
				break;

			case sf::Keyboard::Right:
				generator.nextCircle();
				break;
			case sf::Keyboard::B:
				generator.editCircle(circleRadius, circleSpeed);
				break;
			case sf::Keyboard::M:
				generator.swapMode();
				break;
			case sf::Keyboard::C:
				generator.clear();
				break;
			}
			break;
		}
		}
	}
}

void MovingWindow::update(float deltaTime)
{
	generator.update(deltaTime);
}

void MovingWindow::draw()
{
	generator.draw(window);

	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(static_cast<unsigned int>(20 * zoomLevel));
	text.setPosition(getViewBox().first.x, getViewBox().first.y);
	text.setString("Current Framerate" + std::to_string(framerate));
	window.draw(text);
	text.setPosition(getViewBox().first.x, getViewBox().first.y + text.getCharacterSize() * 2);
	text.setString("New Circle Speed:" + std::to_string(circleSpeed));
	window.draw(text);
	text.setPosition(getViewBox().first.x, getViewBox().first.y + text.getCharacterSize() * 4);
	text.setString("New Circle Radius:" + std::to_string(circleRadius));
	window.draw(text);
}

std::pair<sf::Vector2f, sf::Vector2f> MovingWindow::getViewBox()
{
	sf::View view = window.getView();
	sf::Vector2f winStart = sf::Vector2f{ view.getCenter().x - view.getSize().x / 2,view.getCenter().y - view.getSize().y / 2 };
	sf::Vector2f winEnd = sf::Vector2f{ view.getCenter().x + view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 };

	return { winStart,winEnd };
}
