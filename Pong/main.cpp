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
Vector2f bounce2(Ball pong, Paddle bump);

void playerBallScore(int num);

RenderWindow window;
Paddle playerPad;
Paddle player2Pad;
Paddle player3Pad;
Paddle player4Pad;
Ball pongBall;


Font font;
Texture background;
RectangleShape shape;
Sound ballHit;
SoundBuffer ballHitBuf;
Sound music;
SoundBuffer musicBuf;
Sound victory;
SoundBuffer victoryBuf;
Text startingText;

float speed = 1.0f;

int playerPoints = 0;
int player2Points = 0;
int player3Points = 0;
int player4Points = 0;

int numberOfPlayers = 0;
int playerLastHit = 0;

bool gameOver = false;
bool victorySound = false;

bool onCollisionExitPaddle1;
bool onCollisionExitPaddle2;
bool onCollisionExitPaddle3;
bool onCollisionExitPaddle4;


int main()
{
	window.create(VideoMode(800, 600), "SFML Example");
	Clock clock;

	//ball parameters
	pongBall.ball.setFillColor(Color::Red);
	pongBall.ball.setOrigin(8, 8);
	pongBall.SetStartingPosition(400, 300);
	pongBall.SetVel(100, 300);

	//player 1 bumper parameters
	playerPad.SetSize(15, 60);
	playerPad.SetStartingPosition(25, 100);
	playerPad.bumper.setFillColor(Color::Green);
	playerPad.bumper.setPosition(playerPad.GetPosition());

	//player 2 bumper parameters
	player2Pad.SetSize(15, 60);
	player2Pad.bumper.setFillColor(Color::Blue);
	player2Pad.SetStartingPosition(window.getSize().x - 50, 100);
	player2Pad.bumper.setPosition(player2Pad.GetPosition());

	//player 3 bumper parameters
	player3Pad.SetSize(60, 15);
	player3Pad.bumper.setFillColor(Color::Red);
	player3Pad.SetStartingPosition(window.getSize().x /2, 25);
	player3Pad.bumper.setPosition(player3Pad.GetPosition());

	//player 4 bumper parameters
	player4Pad.SetSize(60, 15);
	player4Pad.bumper.setFillColor(Color::Yellow);
	player4Pad.SetStartingPosition(window.getSize().x /2, window.getSize().y - 50);
	player4Pad.bumper.setPosition(player4Pad.GetPosition());

	//text
	font.loadFromFile("arial.TTF");
	background.loadFromFile("Space Pong Background.png");

	//background image
	shape.setSize(Vector2f(window.getSize().x, window.getSize().y));
	shape.setPosition(0, 0);
	shape.setTexture(&background);

	//ball meets paddle sound effect
	ballHitBuf.loadFromFile("ball hit sound.wav");
	ballHit.setBuffer(ballHitBuf);

	//musical background
	musicBuf.loadFromFile("music.wav");
	music.setBuffer(musicBuf);

	//victory sound effect
	victoryBuf.loadFromFile("victory.wav");
	victory.setBuffer(victoryBuf);

	//creates event, delta time, and updates/renders frames
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
	if (music.getStatus() != SoundSource::Playing)
		music.play();
	//pongBall.getBoundary();
	pongBall.SetDt(dt * speed);

	//controls player 1's bumper
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

	//detects if ball hits sides of screen

	if (numberOfPlayers == 2)
	{
		if (pongBall.GetPosition().y > window.getSize().y && pongBall.GetVel().y > 0)
			pongBall.SetVel(pongBall.GetVel().x, -pongBall.GetVel().y);

		if (pongBall.GetPosition().y < 0 && pongBall.GetVel().y < 0)
			pongBall.SetVel(pongBall.GetVel().x, -pongBall.GetVel().y);
	}
	else if (numberOfPlayers == 4)
	{
		if (pongBall.GetPosition().y > window.getSize().y && pongBall.GetVel().y > 0)
		{
			playerBallScore(playerLastHit);
			pongBall.SetStartingPosition(400, 300);
			speed = 1;
		}

		if (pongBall.GetPosition().y < 0 && pongBall.GetVel().y < 0)
		{
			playerBallScore(playerLastHit);
			pongBall.SetStartingPosition(400, 300);
			speed = 1;
		}
	}

	if (pongBall.GetPosition().x > window.getSize().x && pongBall.GetVel().x > 0)
	{
		playerBallScore(playerLastHit);
		pongBall.SetStartingPosition(400, 300);
		speed = 1;
	}
	if (pongBall.GetPosition().x < 0 && pongBall.GetVel().x < 0)
	{
		playerBallScore(playerLastHit);
		pongBall.SetStartingPosition(400, 300);
		speed = 1;
	}

	//calculates player2's movements
	Vector2f v = pongBall.GetPosition() - player2Pad.GetPosition();
	float len = sqrtf(v.x*v.x + v.y*v.y);
	v /= len;
	player2Pad.SetVelocity(v.y);
	player2Pad.SetPosition(1);

	if (numberOfPlayers == 4)
	{
		//calculates player3's movements
		Vector2f v2 = pongBall.GetPosition() - player3Pad.GetPosition();
		float len2 = sqrtf(v2.x*v2.x + v2.y*v2.y);
		v2 /= len2;
		player3Pad.SetVelocity(v2.x);
		player3Pad.SetPositionX(1);

		//calculates player4's movements
		Vector2f v3 = pongBall.GetPosition() - player4Pad.GetPosition();
		float len3 = sqrtf(v3.x*v3.x + v3.y*v3.y);
		v3 /= len3;
		player4Pad.SetVelocity(v3.x);
		player4Pad.SetPositionX(1);
	}

	//detects ball to bumper collison for player 1
	if (rects_overlap(pongBall.getBoundary(), playerPad.getBoundary()) && onCollisionExitPaddle1)
	{
		onCollisionExitPaddle1 = false;

		Vector2f direction(bounce(pongBall, playerPad).x, bounce(pongBall, playerPad).y);
		float magnitude = sqrt(pongBall.GetVel().x * pongBall.GetVel().x + pongBall.GetVel().y * pongBall.GetVel().y);
		Vector2f newVelocity = direction * magnitude;
		pongBall.SetVel(newVelocity.x, newVelocity.y);

		speed += 0.1;
		playerLastHit = 1;

		if (!gameOver)
			ballHit.play();

	} else {
		onCollisionExitPaddle1 = true;
	}

	//detects ball to bumper collision for player 2
	if (rects_overlap(pongBall.getBoundary(), player2Pad.getBoundary()) && onCollisionExitPaddle2)
	{
		onCollisionExitPaddle2 = false;

		Vector2f direction(bounce(pongBall, player2Pad).x, bounce(pongBall, player2Pad).y);
		float magnitude = sqrt(pongBall.GetVel().x * pongBall.GetVel().x + pongBall.GetVel().y * pongBall.GetVel().y);
		Vector2f newVelocity = direction * magnitude;
		pongBall.SetVel(newVelocity.x, newVelocity.y);

		speed += 0.1;
		playerLastHit = 2;

		if (!gameOver)
			ballHit.play();

	} else {
		onCollisionExitPaddle2 = true;
	}

	if (numberOfPlayers == 4)
	{
		//detects ball to bumper collision for player 3
		if (rects_overlap(pongBall.getBoundary(), player3Pad.getBoundary()) && onCollisionExitPaddle3)
		{
			onCollisionExitPaddle3 = false;

			Vector2f direction(bounce(pongBall, player3Pad).x, bounce(pongBall, player3Pad).y);
			float magnitude = sqrt(pongBall.GetVel().x * pongBall.GetVel().x + pongBall.GetVel().y * pongBall.GetVel().y);
			Vector2f newVelocity = direction * magnitude;
			pongBall.SetVel(newVelocity.x, newVelocity.y);

			speed += 0.1;
			playerLastHit = 3;

			if (!gameOver)
				ballHit.play();

		}
		else {
			onCollisionExitPaddle3 = true;
		}

		//detects ball to bumper collision for player 4
		if (rects_overlap(pongBall.getBoundary(), player4Pad.getBoundary()) && onCollisionExitPaddle3)
		{
			onCollisionExitPaddle4 = false;

			Vector2f direction(bounce(pongBall, player4Pad).x, bounce(pongBall, player4Pad).y);
			float magnitude = sqrt(pongBall.GetVel().x * pongBall.GetVel().x + pongBall.GetVel().y * pongBall.GetVel().y);
			Vector2f newVelocity = direction * magnitude;
			pongBall.SetVel(newVelocity.x, newVelocity.y);

			speed += 0.1;
			playerLastHit = 4;

			if (!gameOver)
				ballHit.play();

		}
		else {
			onCollisionExitPaddle4 = true;
		}
	}

	//if player has more than 5 points, they win, gameover
	if (playerPoints >= 5 || player2Points >= 5 || player3Points >=5 || player4Points >= 5)
		gameOver = true;

	if (gameOver)
	{
		if (victorySound == false)
		{
			victory.play();
			victorySound = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			playerPoints = 0;
			player2Points = 0;
			player3Points = 0;
			player4Points = 0;
			gameOver = false;
			pongBall.SetStartingPosition(400, 300);
			numberOfPlayers = 0;
			victorySound = false;
		}
	}

	if (numberOfPlayers == 0)
	{
		if (Keyboard::isKeyPressed(Keyboard::Num1))
			numberOfPlayers = 2;
		if (Keyboard::isKeyPressed(Keyboard::Num2))
			numberOfPlayers = 4;
	}
	pongBall.SetPosition();
	
	//otherBumper.setPosition(otherBumpPos);

}


void render_frame()
{
	window.clear();

	if (numberOfPlayers != 0)
	{
		if (gameOver == false)
		{
			window.draw(shape);

			window.draw(pongBall.ball);
			window.draw(playerPad.bumper);
			window.draw(player2Pad.bumper);
			if (numberOfPlayers == 4)
			{
				window.draw(player3Pad.bumper);
				window.draw(player4Pad.bumper);
			}

			//player 1 score
			Text playerScore;
			playerScore.setFont(font);
			playerScore.setCharacterSize(50);
			playerScore.setString(to_string(playerPoints));
			playerScore.setFillColor(Color::Green);
			playerScore.setPosition(50, 25);
			window.draw(playerScore);

			//player 2 score
			Text player2Score;
			player2Score.setFont(font);
			player2Score.setCharacterSize(50);
			player2Score.setString(to_string(player2Points));
			player2Score.setFillColor(Color::Blue);
			player2Score.setPosition(window.getSize().x - 75, window.getSize().y - 75);
			window.draw(player2Score);

			if (numberOfPlayers == 4)
			{
				//player 3 score
				Text player3Score;
				player3Score.setFont(font);
				player3Score.setCharacterSize(50);
				player3Score.setString(to_string(player3Points));
				player3Score.setFillColor(Color::Red);
				player3Score.setPosition(window.getSize().x - 75, 25);
				window.draw(player3Score);

				//player 4 score
				Text player4Score;
				player4Score.setFont(font);
				player4Score.setCharacterSize(50);
				player4Score.setString(to_string(player4Points));
				player4Score.setFillColor(Color::Yellow);
				player4Score.setPosition(50, window.getSize().y - 75);
				window.draw(player4Score);
			}

		}

		//gameover text
		Text gameOverText;
		gameOverText.setFont(font);
		gameOverText.setCharacterSize(50);
		gameOverText.setFillColor(Color::White);
		gameOverText.setPosition(75, 400);
		if (gameOver)
		{
			if (playerPoints >= 5)
			{
				gameOverText.setString("Game Over: Player One Wins!");
			}
			if (player2Points >= 5)
			{
				gameOverText.setString("Game Over: Player Two Wins!");
			}
			if (player3Points >= 5)
			{
				gameOverText.setString("Game Over: Player Three Wins!");
			}
			if (player3Points >= 5)
			{
				gameOverText.setString("Game Over: Player Four Wins!");
			}
			window.draw(gameOverText);
		}
	}

	//starting text and 2 vs 4 player choice
	startingText.setFont(font);
	startingText.setCharacterSize(50);
	startingText.setString("Enter 1 for two players,\n enter 2 for 4 players: ");
	startingText.setFillColor(Color::Green);
	startingText.setPosition(75, 300);
	if (numberOfPlayers == 0)
		window.draw(startingText);

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

Vector2f bounce2(Ball pong, Paddle bump)
{
	float min = bump.GetPosition().x - pong.GetRadius();
	float max = bump.GetPosition().x + bump.bumper.getSize().x + pong.GetRadius();
	float f = ((pong.GetPosition().x - min) / (max - min) * 2) - 1;
	float angle = f * 70;
	Vector2f bounceAngle(cos(angle), sin(angle));
	return bounceAngle;
}

void playerBallScore(int num)
{
	switch (num)
	{
	case 1:
		playerPoints++;
		break;
	case 2: 
		player2Points++;
		break;
	case 3: 
		player3Points++;
		break;
	case 4: 
		player4Points++;
		break;
	default:
		break;
	}

}