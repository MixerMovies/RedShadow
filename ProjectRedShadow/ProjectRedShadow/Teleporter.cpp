#include "Teleporter.h"

#include <iostream>
#include "Ray.h"

Teleporter::Teleporter()
{
}


Teleporter::~Teleporter()
{
}

/*ObjModel Teleporter::getModel()
{
	return ObjModel("jr");
}*/

void Teleporter::setCurrentLocation(glm::vec3 location)
{
	_currentLocation = location;
}

void Teleporter::setCurrentRotation(glm::vec3 rotation)
{
	_currentRotation = rotation;
}

glm::vec3 Teleporter::getTeleportLocation()
{
	Ray ray = Ray(_currentLocation, glm::normalize(_currentRotation));
	glm::vec3 location = glm::vec3();
	std::cout << ray.GetYIntersection(0, location) << std::endl;
	return location;
}
