#pragma once

#include "Shader.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#pragma comment(lib, "glew32.lib")

class Gamewindow
{
public:
	Gamewindow();
	~Gamewindow();
	void Setup();
	void Display();
	void NextShader();
	void PreviousShader();
	std::vector<Shader*> shaders;
	int currentshader = 0;
	glm::ivec2 screenSize;
	float rotation = 0;
};
