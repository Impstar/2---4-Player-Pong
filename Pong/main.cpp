#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Ball Class.h"
#include "Paddle Class.h"

using namespace sf;

void update_state(float dt);
void render_frame();

bool rects_overlap(FloatRect r1, FloatRect r2);
Vector2f bounce(Ball pong, Paddle bump);
void opponent_move(Paddle bump);

RenderWindow window;
Paddle otherBumper;
Ball pongBall;
Paddle playerPad;

int main()
{
	window.create(VideoMode(800, 600), "SFML Example");
	Clock clock;

	//_ball.setRadius(8);
	//_ball.setOrigin(16, 16);
	//_ball.setFillColor(Color::Red);
	//_ball.setPosition(400, 300);

	pongBall.ball.setFillColor(Color::Red);
	pongBall.ball.setOrigin(8, 8);
	pongBall.SetStartingPosition(400, 300);
	pongBall.SetVel(100, 300);

	playerPad.SetSize(15, 60);
	playerPad.SetStartingPosition(25, 100);
	playerPad.bumper.setFillColor(Color::White);
	playerPad.bumper.setPosition(playerPad.GetPosition());

	otherBumper.SetSize(15, 60);
	otherBumper.bumper.setFillColor(Color::White);
	otherBumper.SetStartingPosition(window.getSize().x - 25, 100);
	otherBumper.bumper.setPosition(otherBumper.GetPosition());


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		float dt = clock.restart().asSeconds();

		update_state(dt);
		render_frame();
	}

	return 0;
}


void update_state(float dt)
{
	pongBall.getBoundary();
	pongBall.SetDt(dt * 2);
	//Vector2f pos = _ball.getPosition();
	//Vector2f bumpPos = bumper.getPosition();
	//Vector2f otherBumpPos = otherBumper.getPosition();

	if (Keyboard::isKeyPressed(Keyboard::Up) && playerPad.GetPosition().y > 0)
	{
		playerPad.SetVelocity(-600);
		playerPad.SetPosition(dt);
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) && playerPad.GetPosition().y < window.getSize().y - 60)
	{
		playerPad.SetVelocity(600);
		playerPad.SetPosition(dt);
	}

	if (pongBall.GetPosition().y > window.getSize().y && pongBall.GetVel().y > 0)
		pongBall.SetVel(pongBall.GetVel().x, -pongBall.GetVel().y);

	if (pongBall.GetPosition().y < 0 && pongBall.GetVel().y < 0)
		pongBall.SetVel(pongBall.GetVel().x, -pongBall.GetVel().y);

	if (pongBall.GetPosition().x > window.getSize().x && pongBall.GetVel().x > 0)
		pongBall.SetVel(-pongBall.GetVel().x, pongBall.GetVel().y);

	if (pongBall.GetPosition().x < 0 && pongBall.GetVel().x < 0)
		pongBall.SetVel(-pongBall.GetVel().x, pongBall.GetVel().y);

	Vector2f v = pongBall.GetPosition() - otherBumper.GetPosition();
	float len = sqrtf(v.x*v.x + v.y*v.y);
	v /= len;
	otherBumper.SetVelocity(v.y);
	otherBumper.SetPosition(dt + otherBumper.GetPosition().y);

	if (rects_overlap(pongBall.getBoundary(), playerPad.getBoundary()))
	{
		Vector2f direction(bounce(pongBall, playerPad).x, bounce(pongBall, playerPad).y);
		float magnitude = sqrt(pongBall.GetVel().x * pongBall.GetVel().x + pongBall.GetVel().y * pongBall.GetVel().y);
		Vector2f newVelocity = direction * magnitude;
		pongBall.SetVel(newVelocity.x, newVelocity.y);
	}
	if (rects_overlap(pongBall.getBoundary(), otherBumper.getBoundary()))
	{
		Vector2f direction(bounce(pongBall, otherBumper).x, bounce(pongBall, otherBumper).y);
		float magnitude = sqrt(pongBall.GetVel().x * pongBall.GetVel().x + pongBall.GetVel().y * pongBall.GetVel().y);
		Vector2f newVelocity = direction * magnitude;
		pongBall.SetVel(newVelocity.x, newVelocity.y);
	}

	pongBall.SetPosition();
	
	//otherBumper.setPosition(otherBumpPos);

}


void render_frame()
{
	window.clear();
	window.draw(pongBall.ball);
	window.draw(playerPad.bumper);
	window.draw(otherBumper.bumper);
	window.display();
}

bool rects_overlap(FloatRect r1, FloatRect r2)
{
	return !(r1.left + r1.width <= r2.left ||
		r1.left >= r2.left + r2.width ||
		r1.top + r1.height <= r2.top ||
		r1.top >= r2.top + r2.height);
}

Vector2f bounce(Ball pong, Paddle bump)
{
	float min = bump.GetPosition().y - pong.GetRadius();
	float max = bump.GetPosition().y + bump.bumper.getSize().y + pong.GetRadius();
	float f = ((pong.GetPosition().y - min) / (max - min) * 2) - 1;
	float angle = f * 70;
	Vector2f bounceAngle(cos(angle), sin(angle));
	return bounceAngle;
}

void opponent_move(Paddle bump)
{

}