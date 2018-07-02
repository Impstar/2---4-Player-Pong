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


class Ball
{
public:

	//starting information
	Ball();
	CircleShape ball;

	void SetRadius(float rad);
	void SetStartingPosition(float x, float y);
	float GetRadius();
	float GetStartingPositionX();
	float GetStartingPositionY();

	//movement and position
	void SetDt(float dt);
	void SetPosition();
	void SetBallPosition();
	Vector2f GetPosition();
	float GetDt();

	void SetVel(float x, float y);
	Vector2f GetVel();

	FloatRect getBoundary();

private:
	float radius;
	float startingX;
	float startingY;
	float deltaTime;

	Vector2f pos;
	Vector2f vel;

};

Ball::Ball()
{
	radius = 8;
	startingX = 400;
	startingY = 300;
	pos.x = startingX;
	pos.y = startingY;
	ball.setRadius(radius);
}

void Ball::SetRadius(float rad)
{
	radius = rad;
}

void Ball::SetStartingPosition(float x, float y)
{
	startingX = x;
	startingY = y;
	pos.x = startingX;
	pos.y = startingY;
	ball.setPosition(x, y);
}

float Ball::GetRadius()
{
	return radius;
}

float Ball::GetStartingPositionX()
{
	return startingX;
}

float Ball::GetStartingPositionY()
{
	return startingY;
}

//define the unit of frames over time
void Ball::SetDt(float dt)
{
	deltaTime = dt;
}

float Ball::GetDt()
{
	return deltaTime;
}

//sets the position over time
void Ball::SetPosition()
{
	pos += vel * deltaTime;
	ball.setPosition(pos);
}

Vector2f Ball::GetPosition()
{
	return pos;
}

void Ball::SetVel(float x, float y)
{
	vel.x = x;
	vel.y = y;
}

Vector2f Ball::GetVel()
{
	return vel;
}

void Ball::SetBallPosition()
{
	ball.setPosition(pos);
}

//defines the boundary of the ball
FloatRect Ball::getBoundary()
{	
	FloatRect boundary;
	boundary.height = 2 * radius;
	boundary.width = 2 * radius;
	boundary.left = pos.x - radius;
	boundary.top = pos.y - radius;
	return boundary;	
}