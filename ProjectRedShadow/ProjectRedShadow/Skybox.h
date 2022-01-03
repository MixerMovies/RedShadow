#pragma once

#include <gl\glew.h>
#include <glm.hpp>
#include "WorldObject.h"

class Skybox : public WorldObject
{
private:
	void CreateSkybox();
public:
	Skybox(int x, int y, int z, int size);
	void Draw();
	int size;
	~Skybox();
};
