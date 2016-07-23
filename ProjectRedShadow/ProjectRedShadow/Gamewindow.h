#pragma once

#include "GL\freeglut.h"

class Gamewindow
{
public:
	Gamewindow();
	~Gamewindow();
	void Setup(int, int);
	void Display();
};

