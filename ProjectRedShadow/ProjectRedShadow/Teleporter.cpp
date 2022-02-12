#include "Teleporter.h"

#include <iostream>
#include "Ray.h"

Teleporter::Teleporter()
{
}


Teleporter::~Teleporter()
{
}

ObjModel* Teleporter::getModel()
{
	return _teleporterModel;
}

void Teleporter::setCurrentLocation(glm::vec3 location)
{
	_currentLocation = location;
}

glm::vec3 Teleporter::getCurrentLocation()
{
	return _currentLocation;
}

glm::vec3 Teleporter::getCurrentRotation()
{
	return _currentRotation;
}

void Teleporter::setCurrentRotation(glm::vec3 rotation)
{
	_currentRotation = rotation;
}

glm::vec3 Teleporter::getTeleportLocation()
{
	Ray ray = Ray(_currentLocation, glm::normalize(_currentRotation));
	glm::vec3 location = glm::vec3();
	ray.GetYIntersection(0, location);
	return location;
}
