#pragma once

#include <gl\glew.h>
#include <glm.hpp>
#include "WorldObject.h"

class Skybox
{
public:
	Skybox(int x, int y, int z, int size, ObjModel* model);
	int size;
	~Skybox();
	WorldObject object;
};
