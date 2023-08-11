#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>

#include "common.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

float ballXPosition = 0.0f;
float botSpeed = 0.3f;
int posCounter = 0;

void updateBotPosition(float BallX, vec3 position) {
	
	if (BallX <= position.x - 1.0f) {
		setPositionX1(position.x - botSpeed);
	}
	else if (BallX >= position.x + 1.0f) {
		setPositionX1(position.x + botSpeed);
	}

	if (posCounter == 200) {
		SoundEngine->play2D(shoeSource, false);
	}
	else if (posCounter == 400) {
		SoundEngine->play2D(shoe2Source, false);
	}
	posCounter = (posCounter + 1) % 500;
}

void updatePlayerPosition(float BallX, vec3 position) {

	if (BallX <= position.x - 1.0f) {
		setPositionX2(position.x - botSpeed);
	}
	else if (BallX >= position.x + 1.0f) {
		setPositionX2(position.x + botSpeed);
	}

	if (posCounter == 200) {
		SoundEngine->play2D(shoeSource, false);
	}
	else if (posCounter == 400) {
		SoundEngine->play2D(shoe2Source, false);
	}
	posCounter = (posCounter + 1) % 500;
}


float finalBallPosition(vec3 sphereVelocity, vec3 spherePosition) {
	ballXPosition = spherePosition.x + (sphereVelocity.x * 64);
	return ballXPosition;
}