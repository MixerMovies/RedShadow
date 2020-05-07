#pragma once

//#include "ObjModel.h"
#include <glm.hpp>

class Teleporter
{
public:
	Teleporter();
	~Teleporter();
	//ObjModel getModel();
	void setCurrentLocation(glm::vec3 location);
	void setCurrentRotation(glm::vec3 rotation);
	glm::vec3 getTeleportLocation();
private:
	glm::vec3 _currentRotation = { 0,0,0 };
	glm::vec3 _currentLocation = { 0,0,0 };
};
