#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
	_origin = origin;
	_direction = direction;
}

bool Ray::GetYIntersection(int y, glm::vec3& out)
{
	float t = 0;
	glm::vec3 p0 = { 0, y, 0 };
	glm::vec3 normal = { 0, 1, 0 };
	
	float denominator = glm::dot(normal, _direction);
	if (denominator > 0.000001f)
	{
		glm::vec3 p0origin = p0 - _origin;
		t = glm::dot(p0origin, normal) / denominator;
		if (t > 0)
		{
			out = _origin + t * _direction;
			return true;
		}
	}

	return false;
}
