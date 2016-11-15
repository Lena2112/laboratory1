#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

unsigned int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
const float BALL_RADIUS = 10.f, BALL_VELOCITY = 7.f;
const float PADDLE_WIDTH = 120.f, PADDLE_HEIGHT = 20.f, PADDLE_VELOCITY = 6.f;

const float BLOCK_WIDTH = 60.f, BLOCK_HEIGHT = 20.f;
const int COUNT_BLOCS_X = 18, COUNT_BLOCS_Y = 9;

struct Paddle
{
	RectangleShape shape;
	Vector2f velocity;

	Paddle(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({ PADDLE_WIDTH, PADDLE_HEIGHT });
		shape.setFillColor(Color(187, 166, 224));
		shape.setOutlineColor(Color(210, 90, 230));
		shape.setOutlineThickness(1.2f);
		shape.setOrigin({ PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f });
	}

	void update()
	{
		shape.move(velocity);

		if (Keyboard::isKeyPressed(Keyboard::Key::Left) && left() > 0)
		{
			velocity.x = -PADDLE_VELOCITY * 2.f;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < WINDOW_WIDTH)
		{
			velocity.x = PADDLE_VELOCITY * 2.f;
		}
		else
		{
			velocity.x = 0.f;
		}
	}

	float x() 		{ return shape.getPosition().x; }
	float y() 		{ return shape.getPosition().y; }
	float left() 	{ return x() - shape.getSize().x / 2.f; }
	float right() 	{ return x() + shape.getSize().x / 2.f; }
	float top() 	{ return y() - shape.getSize().y / 2.f; }
	float bottom()	{ return y() + shape.getSize().y / 2.f; }
};

struct Ball
{
	CircleShape shape;
	Vector2f velocity{ -BALL_VELOCITY, -BALL_VELOCITY };

	// mX, mY - начальные координаты
	Ball(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setRadius(BALL_RADIUS);
		shape.setFillColor(Color::White);
		shape.setOutlineColor(Color(210, 90, 230));
		shape.setOutlineThickness(1.2f);
	}

	void update()
	{
		shape.move(velocity);

		if (left() < 0)
		{
			velocity.x = BALL_VELOCITY;
		}
		else if (right() > WINDOW_WIDTH)
		{
			velocity.x = -BALL_VELOCITY;
		}

		if (top() < 0)
		{
			velocity.y = BALL_VELOCITY;
		}
		else if (bottom() > WINDOW_HEIGHT)
		{
			velocity.y = -BALL_VELOCITY;
		}
	}

	float x() 		{ return shape.getPosition().x; }
	float y() 		{ return shape.getPosition().y; }
	float left() 	{ return x() - shape.getRadius(); }
	float right() 	{ return x() + shape.getRadius(); }
	float top() 	{ return y() - shape.getRadius(); }
	float bottom()	{ return y() + shape.getRadius(); }
};

struct Brick
{
	RectangleShape shape;
	bool destroyed = false;

	Brick(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
		shape.setFillColor(Color(229, 179, 224));
		shape.setOrigin({ BLOCK_WIDTH / 4.f, BLOCK_HEIGHT / 2.f });
		shape.setOutlineColor(Color(210, 90, 230));
		shape.setOutlineThickness(1.0f);
	}

	float x() 		{ return shape.getPosition().x; }
	float y() 		{ return shape.getPosition().y; }
	float left() 	{ return x() - shape.getSize().x / 2.f; }
	float right() 	{ return x() + shape.getSize().x / 2.f; }
	float top() 	{ return y() - shape.getSize().y / 2.f; }
	float bottom()	{ return y() + shape.getSize().y / 2.f; }
};

struct GameStruct
{
	Paddle paddle{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - PADDLE_HEIGHT / 2.f };
	vector<Brick> bricks;
	Ball mBall{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };
};

// Функция проверяет пересечение двух фигур
template<class T1, class T2> bool isIntersecting(T1& mA, T2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right() &&
		mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

// Функция проверяет пересечение мяча и контроллера
bool TestCollision(GameStruct & game)
{
	if (game.mBall.y() + BALL_RADIUS >= WINDOW_HEIGHT)
	{
		return 0;
	}
		

	if (!isIntersecting(game.paddle, game.mBall))
	{
		return 1;
	}

	game.mBall.velocity.y = -BALL_VELOCITY;

	// Горизонтальное направление мяча меняестся
	// в зависимости от места удара об контроллер
	if (game.mBall.x() < game.paddle.x())
	{
		game.mBall.velocity.x = -BALL_VELOCITY;
	}
	else
	{
		game.mBall.velocity.x = BALL_VELOCITY;
	}
	return 1;
}

void CreateBackground(vector<Brick> & bricks, Texture & t, int & i)
{
	string background[] = { "0.png", "1.png", "2.png", "3.png", "4.png", "5.png" };
	if (bricks.size() % 27 == 0 && i < 6)
	{
		t.loadFromFile(background[i]);
		++i;
	}
}

void TestCollision(Brick & mBrick, GameStruct & game, Texture & t, int & i)
{	
	if (!isIntersecting(mBrick, game.mBall))
	{
		return;
	}

	mBrick.destroyed = true;
	CreateBackground(game.bricks, t, i);

	float overlapLeft{ game.mBall.right() - mBrick.left() };
	float overlapRight{ mBrick.right() - game.mBall.left() };
	float overlapTop{ game.mBall.bottom() - mBrick.top() };
	float overlapBottom{ mBrick.bottom() - game.mBall.top() };

	// Если пересечение слева меньше пересечения справа,
	// можно утверждать, что мяч ударился о блок с левой стороны
	bool ballFromLeft{ abs(overlapLeft) < abs(overlapRight) };

	// Аналогично рассуждаем об ударе о блок сверху или снизу
	bool ballFromTop{ abs(overlapTop) < abs(overlapBottom) };

	// Запомним минимальные пересечения по осям X и Y
	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	// Если магнитуда пересечения по оси X меньше магнитуды по оси Y, мы
	// можем утверждать, что мяч ударился о горизонтальную сторону блока,
	// в противном случае - о вертикальную
	if (abs(minOverlapX) < abs(minOverlapY))
	{
		game.mBall.velocity.x = ballFromLeft ? -BALL_VELOCITY : BALL_VELOCITY;
	}
	else
	{
		game.mBall.velocity.y = ballFromTop ? -BALL_VELOCITY : BALL_VELOCITY;
	}
}

void CreateBackground(Texture & t, Sprite & s)
{
	s.setTexture(t);
	t.loadFromFile("0.png");
	s.setTextureRect(IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
}

void ArkanoidEvents(RenderWindow & window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void Processgame(GameStruct & game, Texture & t, int & i, bool & gameover, bool & victory)
{
	game.paddle.update();
	game.mBall.update();
	for (auto& brick : game.bricks)
	{
		TestCollision(brick, game, t, i);
	}

	game.bricks.erase(
		remove_if(begin(game.bricks), end(game.bricks), [](const Brick& mBrick){ return mBrick.destroyed; }),
		end(game.bricks));
	if (!TestCollision(game))
	{
		// ПРОИГРЫШ
		gameover = true;
	}
	if (game.bricks.size() == 0)
	{
		// ПОБЕДА
		victory = true;
	}
}

void DrawArkanoid(RenderWindow & window, GameStruct & game, Sprite s, bool & gameover, bool & victory)
{
	Font font;
	font.loadFromFile("font.ttf");
	window.clear();

	window.draw(s);
	window.draw(game.paddle.shape);
	window.draw(game.mBall.shape);
	for (auto& brick : game.bricks)
	{
		window.draw(brick.shape);
	}
	if (gameover)
	{
		Text gameOver;
		gameOver.setFont(font);
		gameOver.setCharacterSize(60);
		gameOver.setFillColor(Color::Black);
		gameOver.setStyle(Text::Bold);
		gameOver.setPosition(WINDOW_WIDTH * 2 / 5, WINDOW_HEIGHT * 2 / 5);
		gameOver.setString("game Over");
		window.draw(gameOver);
	}
	if (victory)
	{
		Text Victory;
		Victory.setFont(font);
		Victory.setCharacterSize(60);
		Victory.setFillColor(Color::Black);
		Victory.setStyle(Text::Bold);
		Victory.setPosition(WINDOW_WIDTH * 2 / 5, WINDOW_HEIGHT * 2 / 5);
		Victory.setString("Victory!!!");
		window.draw(Victory);
	}
	window.display();
}

void ProcessMainLoop(RenderWindow & window, GameStruct & game)
{
	Texture t;
	Sprite s;
	int i = 0;
	bool victory = false;
	bool gameover = false;
	CreateBackground(t, s);
	while (window.isOpen())
	{
		if (!gameover && !victory)
		{
			ArkanoidEvents(window);
			Processgame(game, t, i, gameover, victory);
			DrawArkanoid(window, game, s, gameover, victory);
		}
	}
}

int main(int argc, char * argv[])
{
	GameStruct game;
	for (int iX = 0; iX < COUNT_BLOCS_X; ++iX)
	{
		for (int iY = 0; iY < COUNT_BLOCS_Y; ++iY)
		{
			game.bricks.emplace_back((iX + 1) * (BLOCK_WIDTH + 3) + 22, (iY + 2) * (BLOCK_HEIGHT + 3));
		}
	}

	RenderWindow window{ { WINDOW_WIDTH, WINDOW_HEIGHT }, "Arkanoid" };
	window.setFramerateLimit(40);
	
	ProcessMainLoop(window, game);
	return 0;
}

