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

float botPosition() {

	return 0.0f;
}

float finalBallPosition(vec3 sphereVelocity, vec3 spherePosition) {
	//cout << sphereVelocity.z;

	ballXPosition = spherePosition.x + (sphereVelocity.x * 64);
	//cout << ballXPosition;
	return ballXPosition;
}