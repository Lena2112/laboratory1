#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "constsValues.h"
#include <string>
#include <sstream>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>


using namespace sf;
using namespace std;

struct ClockStruct
{
	Font font;
	Music clockTick;
	Texture clockImage;
	Text digit[NUMBER_DIGITS];
	CircleShape dot[NUMBER_DOTS];
	CircleShape clockCircle;
	CircleShape centerCircle;
	RectangleShape hourHand;
	RectangleShape minuteHand;
	RectangleShape secondsHand;
};


void CreateDigits(int i, float angle, ClockStruct & clock, const Vector2f & windowCenter)
{
	float x, y;
	int num = i / 5;
	x = (CLOCK_CIRCLE_SIZE - 10) * cos(angle - (M_PI / 3));
	y = (CLOCK_CIRCLE_SIZE - 10) * sin(angle - (M_PI / 3));

	clock.digit[num].setString(to_string(num + 1));
	clock.digit[num].setFont(clock.font);
	clock.digit[num].setCharacterSize(35);
	clock.digit[num].setFillColor(Color(86, 3, 25));
	clock.digit[num].setStyle(Text::Bold);
	Vector2f positionText(Vector2f((CLOCK_CIRCLE_SIZE - 50) * cos(angle - 1.05), (CLOCK_CIRCLE_SIZE - 50) * sin(angle - 1.05)) + windowCenter);
	clock.digit[num].setOrigin(clock.digit[num].getGlobalBounds().width / 2.0f, clock.digit[num].getGlobalBounds().height);
	clock.digit[num].setPosition(positionText.x, positionText.y);
}

void CreatePoints(const RenderWindow & window, const Vector2f & windowCenter, ClockStruct & clock)
{
	float x, y;
	float angle = 0.0;
	for (int i = 0; i < NUMBER_DOTS; i++)
	{
		x = (CLOCK_CIRCLE_SIZE - 10) * cos(angle);
		y = (CLOCK_CIRCLE_SIZE - 10) * sin(angle);

		if (i % 5 == 0)
		{
			clock.dot[i] = CircleShape(7);
			CreateDigits(i, angle, clock, windowCenter);

		}
		else
		{
			clock.dot[i] = CircleShape(3);
		}
		clock.dot[i].setFillColor(Color(86, 3, 25));
		clock.dot[i].setOrigin(clock.dot[i].getGlobalBounds().width / 2, clock.dot[i].getGlobalBounds().height / 2);
		clock.dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((M_PI * 2) / NUMBER_DOTS);
	}
}

void DrawClockCircle(CircleShape & clockCircle, const RenderWindow & window)
{
	clockCircle.setRadius(CLOCK_CIRCLE_SIZE);
	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(CLOCK_CIRCLE_THICKNESS);
	clockCircle.setOutlineColor(Color(102, 67, 98));
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	clockCircle.setPosition(window.getSize().x / 2 + CLOCK_CIRCLE_THICKNESS, window.getSize().y / 2 + CLOCK_CIRCLE_THICKNESS);

}

void DrawCenterCircle(CircleShape & centerCircle, const Vector2f & windowCenter)
{
	centerCircle.setRadius(10);
	centerCircle.setPointCount(100);
	centerCircle.setFillColor(Color(86, 3, 25));
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
	centerCircle.setPosition(windowCenter);
}

void CreateHands(ClockStruct & clock, const Vector2f & windowCenter)
{
	clock.hourHand.setSize(Vector2f(7, 180));
	clock.minuteHand.setSize(Vector2f(5, 230));
	clock.secondsHand.setSize(Vector2f(2, 270));

	clock.hourHand.setFillColor(Color(102, 67, 98));
	clock.minuteHand.setFillColor(Color(102, 67, 98));
	clock.secondsHand.setFillColor(Color(86, 3, 25));

	clock.hourHand.setOrigin(clock.hourHand.getGlobalBounds().width / 2, clock.hourHand.getGlobalBounds().height - 25);
	clock.minuteHand.setOrigin(clock.minuteHand.getGlobalBounds().width / 2, clock.minuteHand.getGlobalBounds().height - 25);
	clock.secondsHand.setOrigin(clock.secondsHand.getGlobalBounds().width / 2, clock.secondsHand.getGlobalBounds().height - 25);

	clock.hourHand.setPosition(windowCenter);
	clock.minuteHand.setPosition(windowCenter);
	clock.secondsHand.setPosition(windowCenter);
}

void CreateFont(Font & font)
{
	if (!font.loadFromFile("resources/time.ttf"))
	{
		exit(EXIT_FAILURE);
	}
}

void CreateSoundEffect(Music & clockTick)
{
	if (!clockTick.openFromFile("resources/clock-1.wav"))
	{
		exit(EXIT_FAILURE);
	}
	clockTick.setLoop(true);
	clockTick.play();
}

void CreateClockBackground(ClockStruct & clock)
{
	if (!clock.clockImage.loadFromFile("resources/rose.png"))
	{
		exit(EXIT_FAILURE);
	}

	clock.clockCircle.setTexture(&clock.clockImage);
	clock.clockCircle.setTextureRect(sf::IntRect(40, 0, 500, 500));
}

void HandleEvents(RenderWindow & window)
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

void RotateArrows(ClockStruct & clock)
{
	time_t currentTime = time(NULL);
	struct tm * ptm = localtime(&currentTime);

	clock.hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
	clock.minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
	clock.secondsHand.setRotation(ptm->tm_sec * 6);
}

void ProcessMainLoop(RenderWindow & window, ClockStruct & clock)
{
	while (window.isOpen())
	{
		HandleEvents(window);

		RotateArrows(clock);

		window.clear(Color::White);

		window.draw(clock.clockCircle);

		for (int i = 0; i < NUMBER_DOTS; i++)
		{
			window.draw(clock.dot[i]);
			if (i % 5 == 0)
			{
				window.draw(clock.digit[i / 5]);
			}
		}

		window.draw(clock.hourHand);
		window.draw(clock.minuteHand);
		window.draw(clock.secondsHand);
		window.draw(clock.centerCircle);

		window.display();
	}
}

int main()
{
	ClockStruct clock;

    ContextSettings settings;
    settings.antialiasingLevel = 8;

    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Analog Clock", Style::Close, settings);

    Vector2f windowCenter = Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	CreateFont(clock.font);
	CreatePoints(window, windowCenter, clock);
	DrawClockCircle(clock.clockCircle, window);
	DrawCenterCircle(clock.centerCircle, windowCenter);

	CreateHands(clock, windowCenter);

	CreateSoundEffect(clock.clockTick);
	CreateClockBackground(clock);

	ProcessMainLoop(window, clock);

    return EXIT_SUCCESS;
}