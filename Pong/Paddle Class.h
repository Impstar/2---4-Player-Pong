#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace std;
using namespace sf;

class Paddle
{
public:
	Paddle();
	RectangleShape bumper;

	void SetStartingPosition(float x, float y);
	void SetSize(float x, float y);
	void SetPosition(float dt);
	void SetPositionX(float dt);
	Vector2f GetPosition();
	void SetVelocity(float a);

	FloatRect getBoundary();


private:
	float startingX;
	float startingY;

	float vel;

	float deltaTime;

	Vector2f pos;

};

Paddle::Paddle()
{
	pos.x = 25;
	pos.y = 100;
	bumper.setSize(Vector2f(15, 60));
}

void Paddle::SetStartingPosition(float x, float y)
{
	startingX = x;
	startingY = y;
	pos.x = startingX;
	pos.y = startingY;
}

void Paddle::SetSize(float x, float y)
{
	bumper.setSize(Vector2f(x, y));
}

//sets the Y position of paddle over time
void Paddle::SetPosition(float dt)
{
	pos.y += vel * dt;
	bumper.setPosition(pos);
}

//sets the X position of paddle over time
void Paddle::SetPositionX(float dt)
{
	pos.x += vel * dt;
	bumper.setPosition(pos);
}

Vector2f Paddle::GetPosition()
{
	return pos;
}

void Paddle::SetVelocity(float a)
{
	vel = a;
}

//defines the boundary of the paddle
FloatRect Paddle::getBoundary()
{
	FloatRect boundary;
	boundary.height = bumper.getSize().y;
	boundary.width = bumper.getSize().x;
	boundary.left = pos.x;
	boundary.top = pos.y;
	return boundary;
}