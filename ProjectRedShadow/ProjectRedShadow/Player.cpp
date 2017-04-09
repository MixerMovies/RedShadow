#define _USE_MATH_DEFINES

#include "Player.h"
#include <math.h>


Player::Player()
{
}


void Player::goForward()
{
	movingForward = true;
}

void Player::goBackward()
{
	movingBackward = true;
}

void Player::turnLeft(bool turnLeft)
{
	turningLeft = turnLeft;
}

void Player::turnRight(bool turnRight)
{
	turningRight = turnRight;
}

void Player::stop()
{
	movingBackward = false;
	movingForward = false;
}

void Player::move()
{
	if (movingForward)
	{
		position[0] += (float)0.01 * sin(rotation[1] / 180 * M_PI);
		position[1] -= (float)0.01 * sin(rotation[0] / 180 * M_PI);
		position[2] -= (float)0.01 * cos((rotation[1] + rotation[0]) / 180 * M_PI);
	}
	else if (movingBackward)
	{
		position[0] -= (float)0.01 * sin(rotation[1] / 180 * M_PI);
		position[1] += (float)0.01 * sin(rotation[0] / 180 * M_PI);
		position[2] += (float)0.01 * cos((rotation[1] + rotation[0]) / 180 * M_PI);
	}
	if (turningLeft)
	{
		rotation[1]--;
	}
	if (turningRight)
	{
		rotation[1]++;
	}
}

Player::~Player()
{
}
