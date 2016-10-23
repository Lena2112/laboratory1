#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
static const float g = 9.8f;

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.

int GetTheMaximumJumpHeight();
void DisplayStateAtTheMoment(const float &currentTimePoint, const float &v0);
void OutputOnDisplay(const float &timeWhenTheMaximumHeight);
void PpressingExpectation();


int GetTheMaximumJumpHeight()
{
    int theMaximumJumpHeight;
	bool IsValidHeight = false;
	while (!IsValidHeight)
    {
        printf("The maximum jump height: ");
        if (0 == scanf("%d", &theMaximumJumpHeight))
        {
            printf("\nexpected floating-point number\n");
            exit(1);
        }
        if (theMaximumJumpHeight >= 0)
        {
			IsValidHeight = true;
        }
        else
        {
            printf("value must be greater than zero" "\n");
        }
    }
    return theMaximumJumpHeight;
}

void DisplayStateAtTheMoment(const float &currentTimePoint, const float &v0)
{
    double jumpHeight = v0 * currentTimePoint - 0.5 * g * currentTimePoint * currentTimePoint;
    if (jumpHeight >= 0)
    {
        printf("Time=%f, jump height=%f\n", currentTimePoint, jumpHeight);
    }
    return;
}

void OutputOnDisplay(const float &timeWhenTheMaximumHeight)
{
    printf("Time when the maximum height:%f\n", timeWhenTheMaximumHeight);
    float v0 = g * timeWhenTheMaximumHeight;
    bool passedHalfWay = false;
    for (float currentTimePoint = 0; currentTimePoint < timeWhenTheMaximumHeight * 2; currentTimePoint += 0.1f)
    {
        if (currentTimePoint > timeWhenTheMaximumHeight && !passedHalfWay)
        {
            passedHalfWay = true;
            DisplayStateAtTheMoment(timeWhenTheMaximumHeight, v0);
        }
        DisplayStateAtTheMoment(currentTimePoint, v0);
    }
    DisplayStateAtTheMoment(timeWhenTheMaximumHeight * 2, v0);
    return;
}

int main()
{
    int theMaximumJumpHeight = GetTheMaximumJumpHeight();
    float timeWhenTheMaximumHeight = sqrt(theMaximumJumpHeight * 2 / g);
    OutputOnDisplay(timeWhenTheMaximumHeight);    

    return 0;
}

