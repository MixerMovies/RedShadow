#pragma once

#include "framework.h"

#include <glm.hpp>
#include <array>

class RED_EXPORT Player
{
public:
	Player();
	float speed = 0.01f;
	float sprintingMultiplier = 10.0f;
	bool isSprinting = false;
	glm::vec3 position = { 0, -1.6, 0 };
	glm::vec3 rotation = { 0, 0, 0 };
	glm::vec2 mousePositionOffset = { 0, 0 };
	void GoForward();
	void GoBackward();
	void TurnLeft(bool);
	void TurnRight(bool);
	void MouseEnabled(bool);
	void Stop();
	void Move(int elapsedTime);
	~Player();
private:
	bool _movingForward = false;
	bool _movingBackward = false;
	bool _turningLeft = false;
	bool _turningRight = false;
	bool _mouseActivated = false;
	glm::vec2 _mouseStartingPosition = { 0, 0 };
};

