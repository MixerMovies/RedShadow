#pragma once

#include "framework.h"

#include <gl/glew.h>
#include "ObjModel.h"
#include <glm.hpp>

class RED_EXPORT Teleporter
{
public:
	bool startTeleporting = false;
	Teleporter();
	~Teleporter();
	ObjModel* getModel();
	void setCurrentLocation(glm::vec3 location);
	glm::vec3 getCurrentLocation();
	glm::vec3 getCurrentRotation();
	void setCurrentRotation(glm::vec3 rotation);
	glm::vec3 getTeleportLocation();
private:
	ObjModel* _teleporterModel = new ObjModel("models/Projects/teleporter/Teleporter.obj");
	glm::vec3 _currentRotation = { 0,0,0 };
	glm::vec3 _currentLocation = { 0,0,0 };
};
