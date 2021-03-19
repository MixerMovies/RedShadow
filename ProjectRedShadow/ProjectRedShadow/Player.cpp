#define _USE_MATH_DEFINES

#include "Player.h"
#include <math.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>

Player::Player()
{
}


void Player::GoForward()
{
	_movingForward = true;
}

void Player::GoBackward()
{
	_movingBackward = true;
}

void Player::TurnLeft(bool turnLeft)
{
	_turningLeft = turnLeft;
}

void Player::TurnRight(bool turnRight)
{
	_turningRight = turnRight;
}

void Player::MouseEnabled(bool mouseActivated)
{
	_mouseActivated = mouseActivated;
}

void Player::Stop()
{
	_movingBackward = false;
	_movingForward = false;
}

void Player::Move(int elapsedTime)
{
	/*if (_mouseActivated)
	{
		
	}*/
	if (_movingForward)
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
	else if (_movingBackward)
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
	if (_turningLeft)
	{
		rotation[1] -= (float) 0.005 * elapsedTime;
	}
	if (_turningRight)
	{
		rotation[1] += (float) 0.005 * elapsedTime;
	}
}

Player::~Player()
{
}
