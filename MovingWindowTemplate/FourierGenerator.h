#pragma once
#include <SFML/Graphics.hpp>


class FourierGenerator
{
public:
	FourierGenerator();
	void update(float deltaTime);
	void draw(sf::RenderTarget& target);
	void addCircle(float radius = 0.0f, float rSpeed = 0.0f);
	void clear();
	void fullClear();
	void nextCircle();
	void prevCircle();
	void editCircle(float radius, float rSpeed);
	void swapMode() { waves = !waves; clear(); }
private:
	std::vector<sf::CircleShape> bigCircles;
	std::vector<sf::CircleShape> smallCircles;
	std::vector<std::pair<float, float>> circleMov;
	sf::VertexArray lines;
	sf::VertexArray points;
	bool waves{ true };
	int activeCircle{ -1 };
};