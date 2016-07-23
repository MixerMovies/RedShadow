#pragma once

#include <array>

class Player
{
public:
	Player();
	std::array<float, 3> position = { 0, -5, 0 };
	std::array<float, 3> rotation = { 0, 0, 0 };
	void goForward();
	void goBackward();
	void turnLeft(bool);
	void turnRight(bool);
	void stop();
	void move();
	~Player();
private:
	bool movingForward = false;
	bool movingBackward = false;
	bool turningLeft = false;
	bool turningRight = false;
};

