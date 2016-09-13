#include <stdafx.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.
//
// TODO: Fix all warnings on high warning level (/W4, -Wall -Wextra).
// TODO: Rename variables and split to several functions,
// see also https://ps-group.github.io/sfml/coding_conventions.html
// TODO: fix negative height values, fix heigh values higher than max height.

int GetTheMaximumJumpHeight()
{
	int theMaximumJumpHeight;
	bool flag = false;
	while (!flag)
	{
	    printf("The maximum jump height: ");
	    if (0 == scanf("%d", &theMaximumJumpHeight))
	    {
		    printf("\nexpected floating-point number\n");
		    exit(1);
	    }
		if (theMaximumJumpHeight > 0 && theMaximumJumpHeight < INT_MAX)
		{
			flag = true;
		}
		else
		{
			printf("value must be greater than zero" "\n");
		}
	}
	return theMaximumJumpHeight;
}

void DisplayStateAtTheMoment(const float currentTimePoint, const float v0, const float g)
{
	double jumpHeight = v0 * currentTimePoint - 0.5 * g * currentTimePoint * currentTimePoint;
	if (jumpHeight >= 0)
	{
		printf("Time=%f, jump height=%f\n", currentTimePoint, jumpHeight);
	}
	return;
}

void OutputOnDisplay(const float timeWhenTheMaximumHeight, const float g)
{
	printf("Time when the maximum height:%f\n", timeWhenTheMaximumHeight);
	float v0 = g * timeWhenTheMaximumHeight;
	bool passedHalfWay = false;
	for (float currentTimePoint = 0; currentTimePoint < timeWhenTheMaximumHeight * 2; currentTimePoint += 0.1f)
	{
		if (currentTimePoint > timeWhenTheMaximumHeight && !passedHalfWay)
		{
			passedHalfWay = true;
			DisplayStateAtTheMoment(timeWhenTheMaximumHeight, v0, g);
		}
		DisplayStateAtTheMoment(currentTimePoint, v0, g);
	}
	DisplayStateAtTheMoment(timeWhenTheMaximumHeight * 2, v0, g);
	return;
}

void PpressingExpectation()
{
	printf("To exit, press any key");
	getch();
}

int main(int, char *[])
{
    const float g = 9.8f;
	int theMaximumJumpHeight = GetTheMaximumJumpHeight();
	float timeWhenTheMaximumHeight = sqrt(theMaximumJumpHeight * 2 / g);
	OutputOnDisplay(timeWhenTheMaximumHeight, g);	

	PpressingExpectation();
	return 0;
}

