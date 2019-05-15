#pragma once

#include "Shader.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <vector>
#include "Space.h"
#pragma comment(lib, "glew32.lib")
#include <openvr.h>

class Gamewindow
{
public:
	struct EyeTextures
	{
		GLuint leftEye;
		GLuint rightEye;
	};

	Gamewindow(Space* space, vr::IVRSystem* vrSystem);
	~Gamewindow();
	void initVRShaders();
	EyeTextures Display();
	void NextShader();
	void PreviousShader();
	void NextPostShader();
	void PreviousPostShader();
	void setVRSystem(vr::IVRSystem*);
	void RenderWorld(glm::mat4);
	std::vector<Shader*> shaders;
	std::vector<Shader*> postProcessingShaders;
	int currentshader = 0;
	int currentPostShader = 0;
	glm::ivec2 screenSize;
	float rotation = 0;
	bool postProcessingEnabled = false;
};
