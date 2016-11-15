#include "stdafx.h"
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <concrt.h>
#include <cmath>

using namespace sf;
using namespace std;


const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const size_t RECTANGLE_AMOUNT = 6;
const Color STARTER_COLOR = Color(32, 227, 149);
const int RECTANGLE_STARTER_WIDTH = 30;
const int RECTANGLE_STARTER_HEIGHT = 30;
const int STARTER_PADDING_X = 20;
const int STARTER_PADDING_Y = 30;
const int PADDING_BETWEEN_RECTANGLES = 5;
const float SPEED = 0.1;

struct Blocks
{
	RectangleShape rectangle[RECTANGLE_AMOUNT];
	Vector2f size;
	Color color;
};



void LeedToEndPos(Blocks &blocks, size_t i, float &x)
{
	for (i; i < RECTANGLE_AMOUNT; ++i)
	{
		x = blocks.rectangle[i].getPosition().x + SPEED;
		blocks.rectangle[i].setPosition(x, blocks.rectangle[i].getPosition().y);
	}
}
void Motion(Blocks &blocks, int &mode, float &changer, vector <float> &asynchStarterPosY)
{
	float x = 0;
	float y = 0;
	Color currentColor;
	for (int i = 0; i < RECTANGLE_AMOUNT; ++i)
	{
		if (mode == 0)
		{
			if (x + RECTANGLE_STARTER_WIDTH / 2 < SCREEN_WIDTH && x - RECTANGLE_STARTER_WIDTH / 2 > 0)
			{
				changer = 0;
				LeedToEndPos(blocks, i, x);
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x;
				y = blocks.rectangle[i].getPosition().y;
				currentColor = Color(32, 64, int(x) % 255);
				++mode;
			}

		}
		if (mode == 1)
		{
			if (y + RECTANGLE_STARTER_HEIGHT / 2 < SCREEN_HEIGHT && y - RECTANGLE_STARTER_HEIGHT / 2 > 0)
			{
				changer = 0;
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x + SPEED;
				y = blocks.rectangle[i].getPosition().y;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT - changer);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(blocks.rectangle[0].getPosition().y) % 255);
				++mode;
			}

		}
		if (mode == 2)
		{
			if (x + RECTANGLE_STARTER_WIDTH / 2 < SCREEN_WIDTH && x - RECTANGLE_STARTER_WIDTH / 2 > 0)
			{
				changer = 0;
				LeedToEndPos(blocks, i, x);
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x + SPEED; // *0.1 * time;
				y = blocks.rectangle[i].getPosition().y;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH - changer, RECTANGLE_STARTER_HEIGHT);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(x) % 255);
				++mode;
			}
		}
		if (mode == 3)
		{
			if (y + RECTANGLE_STARTER_HEIGHT / 2 < SCREEN_HEIGHT && y - RECTANGLE_STARTER_HEIGHT / 2 > 0)
			{
				changer = 0;
				for (size_t j = 0; j < RECTANGLE_AMOUNT; ++j)
				{
					asynchStarterPosY[j] = blocks.rectangle[j].getPosition().y;
				}
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x;
				y = blocks.rectangle[i].getPosition().y + SPEED;
				currentColor = Color(32, 64, int(blocks.rectangle[0].getPosition().y) % 255);
				++mode;
			}
		}
		if (mode == 4)
		{
			if (x == STARTER_PADDING_X + blocks.rectangle[i].getOrigin().x)
			{
				changer = 0;
				LeedToEndPos(blocks, i, x);
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x - SPEED;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(x) % 255);

			}
		}
		if (mode == 5)
		{
			if (y + RECTANGLE_STARTER_HEIGHT / 2 < SCREEN_HEIGHT && y - RECTANGLE_STARTER_HEIGHT / 2 > 0)
			{
				mode = 0;
				changer = 0;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT);
				for (size_t j = 0; j < RECTANGLE_AMOUNT; ++j)
				{
					blocks.rectangle[j].setSize(blocks.size);
					blocks.rectangle[j].setPosition(STARTER_PADDING_X + blocks.rectangle[j].getOrigin().x, (((RECTANGLE_STARTER_HEIGHT + PADDING_BETWEEN_RECTANGLES) * j) + STARTER_PADDING_Y + blocks.rectangle[j].getOrigin().y));
				}
			}
			else
			{
				x = blocks.rectangle[i].getPosition().x;
				y = blocks.rectangle[i].getPosition().y - SPEED;
				blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH - changer, RECTANGLE_STARTER_HEIGHT);
				blocks.rectangle[i].setSize(blocks.size);
				currentColor = Color(32, 64, int(blocks.rectangle[0].getPosition().y) % 255);
			}
		}
		if (!(mode == 0 && changer == 0))
		{
			blocks.rectangle[i].setPosition(x, y);
		}
		blocks.rectangle[i].setFillColor(currentColor);
	}
}

void DrawFrame(Blocks &blocks, RenderWindow &window)
{
	for (int i = 0; i < RECTANGLE_AMOUNT; ++i)
	{
		window.draw(blocks.rectangle[i]);
	}
	window.display();
}

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Animation", Style::Close, settings);
	Blocks blocks;
	blocks.size = Vector2f(RECTANGLE_STARTER_WIDTH, RECTANGLE_STARTER_HEIGHT);
	blocks.color = STARTER_COLOR;
	float changer = 0;
	int mode = 1;
	vector <float> asynchStarterPosY(RECTANGLE_AMOUNT);
	for (size_t i = 0; i < RECTANGLE_AMOUNT; ++i)
	{
		blocks.rectangle[i].setSize(blocks.size);
		blocks.rectangle[i].setFillColor(blocks.color);
		blocks.rectangle[i].setOrigin(RECTANGLE_STARTER_WIDTH / 2, RECTANGLE_STARTER_HEIGHT / 2);
		blocks.rectangle[i].setPosition(STARTER_PADDING_X + blocks.rectangle[i].getOrigin().x, (((RECTANGLE_STARTER_HEIGHT + PADDING_BETWEEN_RECTANGLES) * i) + STARTER_PADDING_Y + blocks.rectangle[i].getOrigin().y));
	}
	while (window.isOpen())
	{
		window.clear(Color::White);
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		changer = changer + 0.001;
		Motion(blocks, mode, changer, asynchStarterPosY);
		DrawFrame(blocks, window);
	}
    return 0;
}