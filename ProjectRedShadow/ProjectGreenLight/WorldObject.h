#pragma once

#include "../ProjectRedShadow/ObjModel.h"
#include <glm.hpp>

class WorldObject
{
public:
	ObjModel* objModel = nullptr;
	std::string name;
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;
};
