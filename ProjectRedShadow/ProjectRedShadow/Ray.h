#pragma once

#include "glm.hpp"

class Ray
{
private:
	glm::vec3 _origin;
	glm::vec3 _direction;

public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	bool GetYIntersection(int y, glm::vec3& out);
};

