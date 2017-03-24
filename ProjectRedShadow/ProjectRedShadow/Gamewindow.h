#pragma once

#include "Shader.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class Gamewindow
{
public:
	Gamewindow();
	~Gamewindow();
	void Setup(int, int);
	void Display();
	Shader* shader;
	glm::ivec2 screenSize;
	float rotation = 0;
};
