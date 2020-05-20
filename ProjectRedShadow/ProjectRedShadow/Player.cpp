#define _USE_MATH_DEFINES

#include "Player.h"
#include <math.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

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

void Player::move(int elapsedTime)
{
	if (movingForward)
	{
		glm::mat4 rotMat = glm::translate(glm::mat4(), position);
		rotMat = glm::rotate(rotMat, rotation.x, glm::vec3(1, 0, 0));
		rotMat = glm::rotate(rotMat, -rotation.y, glm::vec3(0, 1, 0));
		rotMat = glm::rotate(rotMat, rotation.z, glm::vec3(0, 0, 1));
		if(isSprinting)
			rotMat = glm::translate(rotMat, glm::vec3(0, 0, -speed * sprintingMultiplier * elapsedTime));
		else
			rotMat = glm::translate(rotMat, glm::vec3(0, 0, -speed * elapsedTime));
		
		glm::vec4 pos = rotMat * glm::vec4(0, 0, 0, 1);
		position = glm::vec3(pos.x, pos.y, pos.z);
	}
	else if (movingBackward)
	{
		glm::mat4 rotMat = glm::translate(glm::mat4(), position);
		rotMat = glm::rotate(rotMat, rotation.x, glm::vec3(1, 0, 0));
		rotMat = glm::rotate(rotMat, -rotation.y, glm::vec3(0, 1, 0));
		rotMat = glm::rotate(rotMat, rotation.z, glm::vec3(0, 0, 1));
		if(isSprinting)
			rotMat = glm::translate(rotMat, glm::vec3(0, 0, speed * sprintingMultiplier * elapsedTime));
		else
			rotMat = glm::translate(rotMat, glm::vec3(0, 0, speed * elapsedTime));

		glm::vec4 pos = rotMat * glm::vec4(0, 0, 0, 1);
		position = glm::vec3(pos.x, pos.y, pos.z);
	}
	if (turningLeft)
	{
		rotation[1] -= (float) 0.005 * elapsedTime;
	}
	if (turningRight)
	{
		rotation[1] += (float) 0.005 * elapsedTime;
	}
}

Player::~Player()
{
}
