#pragma once

#include <glm.hpp>
#include <array>

class Player
{
public:
	Player();
	glm::vec3 position = { 0, 0, 50 };
	glm::vec3 rotation = { 0, 0, 0 };
	void goForward();
	void goBackward();
	void turnLeft(bool);
	void turnRight(bool);
	void stop();
	void move(int elapsedTime);
	~Player();
private:
	bool movingForward = false;
	bool movingBackward = false;
	bool turningLeft = false;
	bool turningRight = false;
};

