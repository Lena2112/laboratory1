#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

unsigned int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
const float BALL_RADIUS = 10.f, BALL_VELOCITY = 8.f;
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
			velocity.x = -PADDLE_VELOCITY * 2.f;
		else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < WINDOW_WIDTH)
			velocity.x = PADDLE_VELOCITY * 2.f;
		else
			velocity.x = 0.f;
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
			velocity.x = BALL_VELOCITY;
		else if (right() > WINDOW_WIDTH)
			velocity.x = -BALL_VELOCITY;

		if (top() < 0)
			velocity.y = BALL_VELOCITY;
		else if (bottom() > WINDOW_HEIGHT)
			velocity.y = -BALL_VELOCITY;
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


// Функция проверяет пересечение двух фигур
template<class T1, class T2> bool isIntersecting(T1& mA, T2& mB)
{
	return mA.right() >= mB.left() && mA.left() <= mB.right() &&
		mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

// Функция проверяет пересечение мяча и контроллера
bool TestCollision(Paddle& mPaddle, Ball& mBall)
{
	if (mBall.y() + BALL_RADIUS >= WINDOW_HEIGHT)
		return 0;

	if (!isIntersecting(mPaddle, mBall))
		return 1;

	mBall.velocity.y = -BALL_VELOCITY;

	// Горизонтальное направление мяча меняестся
	// в зависимости от места удара об контроллер
	if (mBall.x() < mPaddle.x())
		mBall.velocity.x = -BALL_VELOCITY;
	else
		mBall.velocity.x = BALL_VELOCITY;
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

void TestCollision(Brick & mBrick, Ball & mBall, vector<Brick> & bricks, Texture & t, int & i)
{	
	if (!isIntersecting(mBrick, mBall))
		return;

	mBrick.destroyed = true;
	CreateBackground(bricks, t, i);

	float overlapLeft{ mBall.right() - mBrick.left() };
	float overlapRight{ mBrick.right() - mBall.left() };
	float overlapTop{ mBall.bottom() - mBrick.top() };
	float overlapBottom{ mBrick.bottom() - mBall.top() };

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
		mBall.velocity.x = ballFromLeft ? -BALL_VELOCITY : BALL_VELOCITY;
	else
		mBall.velocity.y = ballFromTop ? -BALL_VELOCITY : BALL_VELOCITY;
}

void CreateBackground(Texture & t, Sprite & s)
{
	s.setTexture(t);
	t.loadFromFile("0.png");
	s.setTextureRect(IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
}

int main(int argc, char * argv[])
{
	Paddle paddle{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - PADDLE_HEIGHT / 2.f };
	Ball ball{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };

	vector<Brick> bricks;
	for (int iX = 0; iX < COUNT_BLOCS_X; ++iX)
		for (int iY = 0; iY < COUNT_BLOCS_Y; ++iY)
			bricks.emplace_back((iX + 1) * (BLOCK_WIDTH + 3) + 22, (iY + 2) * (BLOCK_HEIGHT + 3));

	RenderWindow window{ { WINDOW_WIDTH, WINDOW_HEIGHT }, "Arkanoid" };
	window.setFramerateLimit(40);

	Texture t;
	Sprite s;
	int i = 0;
	CreateBackground(t, s);
	while (window.isOpen())
	{
		window.clear();
			
		window.draw(s);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		paddle.update();
		ball.update();
		if (!TestCollision(paddle, ball))
		{
			// ПРОИГРЫШ
		}
		for (auto& brick : bricks)
			TestCollision(brick, ball, bricks, t, i);

		bricks.erase(
			remove_if(begin(bricks), end(bricks), [](const Brick& mBrick){ return mBrick.destroyed; }),
			end(bricks));	

		if (bricks.size() == 0)
		{
			// ПОБЕДА
		}

		window.draw(paddle.shape);
		window.draw(ball.shape);
		for (auto& brick : bricks)
			window.draw(brick.shape);

		window.display();
	}

	return 0;
}

