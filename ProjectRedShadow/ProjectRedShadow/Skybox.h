#pragma once

#include <gl\glew.h>
#include <glm.hpp>
#include "ObjModel.h"

class Skybox
{
public:
	Skybox(int x, int y, int z, ObjModel* model);
	~Skybox();
	void draw();
	int x;
	int y;
	int z;
	ObjModel* model;
};
