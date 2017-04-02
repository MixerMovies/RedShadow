#pragma once

#include "Shader.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>

class Gamewindow
{
public:
	Gamewindow();
	~Gamewindow();
	void Setup(int, int);
	void Display();
	void NextShader();
	void PreviousShader();
	std::vector<Shader*> shaders;
	int currentshader = 0;
	glm::ivec2 screenSize;
	float rotation = 0;
};
