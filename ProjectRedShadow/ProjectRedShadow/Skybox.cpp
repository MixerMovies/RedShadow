#include "Skybox.h"

#include <vector>

Skybox::Skybox(int x, int y, int z, int size, ObjModel* model)
{
	Skybox::size = size;
	object = WorldObject();
	object.location = glm::vec3(x, y, z);
	object.rotation = glm::vec3(0, 0, 0);
	object.objModel = model;
	object.name = "Skybox";
	object.scale = glm::vec3(size, size, size);
}


Skybox::~Skybox()
{
}
