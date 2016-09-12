#include <stdafx.h>
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


int main(int, char *[])
{
	const float g = 9.8f;
	float timeWhenTheMaximumHeight;
	int theMaximumJumpHeight;
	printf("The maximum jump height: ");
	if (0 == scanf("%d", &theMaximumJumpHeight))
	{
		printf("\nexpected floating-point number\n");
		exit(1);
	}

	timeWhenTheMaximumHeight = sqrt(theMaximumJumpHeight * 2 / g);
	printf("time when the maximum height=%f\n", timeWhenTheMaximumHeight);
	bool passedHalfWay = false;
	for (float currentTimePoint = 0; currentTimePoint < timeWhenTheMaximumHeight * 2; currentTimePoint += 0.1f)
	{
		if (currentTimePoint > timeWhenTheMaximumHeight && !passedHalfWay)
		{
			passedHalfWay = true;
			float v0 = g * timeWhenTheMaximumHeight;
			float jumpHeight = v0 * timeWhenTheMaximumHeight - 0.5 * g * timeWhenTheMaximumHeight * timeWhenTheMaximumHeight;
			printf("time when the maximum height=%f, the maximum jump height=%f\n", timeWhenTheMaximumHeight, jumpHeight);
		}
		float v0 = g * timeWhenTheMaximumHeight;
		float jumpHeight = v0 * currentTimePoint - 0.5 * g * currentTimePoint * currentTimePoint;
		printf("time=%f, jump height=%f\n", currentTimePoint, jumpHeight);
	}

	float v0 = g * timeWhenTheMaximumHeight;
	float jumpHeight = v0 * (timeWhenTheMaximumHeight * 2) - 0.5 * g * (timeWhenTheMaximumHeight * 2) * (timeWhenTheMaximumHeight * 2);
	printf("time of the end jump=%f, jump height=%f\n", timeWhenTheMaximumHeight * 2, jumpHeight);

	system("pause");

	return 0;
}

