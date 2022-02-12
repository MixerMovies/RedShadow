#pragma once

#include <glm.hpp>
#include "../ProjectRedShadow/ObjModel.h"

class Light
{
public:
	ObjModel* model = new ObjModel("models/Projects/star/star.obj");
	glm::vec3 position = glm::vec3(1, 1, 0);
	glm::vec3 color = glm::vec3(1, 1, 1);
};

