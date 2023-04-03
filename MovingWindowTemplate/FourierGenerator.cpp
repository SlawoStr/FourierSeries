#include "FourierGenerator.h"
#define PI 3.14159265

FourierGenerator::FourierGenerator() : lines(sf::Lines),points(sf::Points)
{

}

void FourierGenerator::update(float deltaTime)
{
	// Update small Circles position
	for (int i = 0; i < smallCircles.size(); i++)
	{
		// Update circle angle
		circleMov[i].first += circleMov[i].second;
		// Reset angle if above or below 360 degress
		if (circleMov[i].first > 360.0f)
		{
			circleMov[i].first -= 360.0f;
		}
		else if (circleMov[i].first < 360.0f)
		{
			circleMov[i].first += 360.0f;
		}
		sf::Vector2f position;
		position.x = static_cast<float>((bigCircles[i].getPosition().x + bigCircles[i].getRadius() * std::sin(circleMov[i].first * PI / 180)));
		position.y = static_cast<float>((bigCircles[i].getPosition().y + bigCircles[i].getRadius() * std::cos(circleMov[i].first * PI / 180)));
		smallCircles[i].setPosition(position);
	}
	// Update big circles
	for (int i = 1; i < bigCircles.size(); i++)
	{
		bigCircles[i].setPosition(smallCircles[i - 1].getPosition());
	}
	// Update Lines
	for (int i = 1; i < smallCircles.size(); i++)
	{
		lines[(i-1) * 2].position = smallCircles[i - 1].getPosition();
		lines[(i-1) * 2 + 1].position = smallCircles[i].getPosition();
	}
	// Update Points
	if (!smallCircles.empty())
	{
		if (!waves)
		{
			sf::Vertex point;
			point.color = sf::Color::Blue;
			point.position = smallCircles.back().getPosition();
			points.append(point);
		}
		else
		{
			sf::Vertex point;
			point.color == sf::Color::Red;
			point.position.x = 100.f;
			point.position.y = smallCircles.back().getPosition().y;
			points.append(point);
		}
	}
	if (waves)
	{

		for (int i = 0; i < points.getVertexCount(); i++)
		{
			points[i].position.x += 1.0f;
		}
	}
}

void FourierGenerator::draw(sf::RenderTarget& target)
{
	for (int i = 0; i < bigCircles.size(); ++i)
	{
		target.draw(bigCircles[i]);
	}
	for (int i = 0; i < smallCircles.size(); ++i)
	{
		target.draw(smallCircles[i]);
	}
	target.draw(lines);
	target.draw(points);
}

void FourierGenerator::addCircle(float radius, float rSpeed)
{
	if (radius == 0.0f)
	{
		if (bigCircles.empty())
		{
			radius = 5000.0f;
		}
		else
		{
			radius = bigCircles.back().getRadius() * 0.9f;
		}
	}
	if (rSpeed == 0.0f)
	{
		if (circleMov.empty())
		{
			rSpeed = -1.0f;
		}
		else
		{
			rSpeed =  circleMov.back().second * 0.99f ;
			rSpeed *= -1;
		}
	}
	sf::CircleShape bCircle;
	bCircle.setFillColor(sf::Color::Transparent);
	bCircle.setRadius(radius);
	bCircle.setOrigin(sf::Vector2f(radius, radius));
	bCircle.setOutlineThickness(1.0f);
	bCircle.setOutlineColor(sf::Color::White);
	// IF empty create first circle on default position
	if (bigCircles.empty())
	{
		bCircle.setPosition(sf::Vector2f(0.0f, 0.0f));
	}
	// If not create big circle on small circle
	else
	{
		bCircle.setPosition(smallCircles.back().getPosition());
	}
	bigCircles.push_back(bCircle);


	sf::CircleShape sCircle;
	sCircle.setFillColor(sf::Color::Red);
	sCircle.setRadius(5.0f);
	sCircle.setOrigin(sf::Vector2f(5.0f, 5.0f));

	sf::Vector2f circlePos;
	circlePos.x = static_cast<float>((bCircle.getRadius() * std::sin(90.0f * PI / 180))) + bCircle.getPosition().x;
	circlePos.y = static_cast<float>((bCircle.getRadius() * std::cos(90.0f * PI / 180))) + bCircle.getPosition().y;
	sCircle.setPosition(circlePos);
	smallCircles.push_back(sCircle);
	circleMov.push_back(std::make_pair(90.0f, rSpeed));

	sf::Vertex line[2];
	line[0].color = sf::Color::White;
	line[1].color = sf::Color::White;
	lines.append(line[0]);
	lines.append(line[1]);

	if (activeCircle == -1)
	{
		activeCircle = 0;
		nextCircle();
	}
}

void FourierGenerator::clear()
{
	points.clear();
}

void FourierGenerator::fullClear()
{
	clear();

	for (int i = 0; i < smallCircles.size(); i++)
	{
		sf::Vector2f circlePos;
		circlePos.x = static_cast<float>((bigCircles[i].getRadius() * std::sin(90.0f * PI / 180))) + bigCircles[i].getPosition().x;
		circlePos.y = static_cast<float>((bigCircles[i].getRadius() * std::cos(90.0f * PI / 180))) + bigCircles[i].getPosition().y;
		smallCircles[i].setPosition(circlePos);
		circleMov[i].first = 90.0f;
		if (i != smallCircles.size() - 1)
		{
			bigCircles[i + 1].setPosition(circlePos);
		}
	}
}

void FourierGenerator::nextCircle()
{
	if (activeCircle == -1)
		return;
	int nextActive = activeCircle + 1;
	if (nextActive == bigCircles.size())
	{
		nextActive = 0;
	}
	bigCircles[activeCircle].setOutlineColor(sf::Color::White);
	bigCircles[nextActive].setOutlineColor(sf::Color::Red);
	activeCircle = nextActive;
}

void FourierGenerator::prevCircle()
{
	if (activeCircle == -1)
		return;
	int nextActive = activeCircle - 1;
	if (nextActive < 0)
	{
		nextActive = static_cast<int>(bigCircles.size()-1);
	}
	bigCircles[activeCircle].setOutlineColor(sf::Color::White);
	bigCircles[nextActive].setOutlineColor(sf::Color::Red);
	activeCircle = nextActive;
}

void FourierGenerator::editCircle(float radius, float rSpeed)
{
	clear();
	bigCircles[activeCircle].setRadius(radius);
	bigCircles[activeCircle].setOrigin(sf::Vector2f(radius, radius));
	circleMov[activeCircle].second = rSpeed;
}

