#include "Space.h"

Space::Space()
{
	worldModels = std::vector<WorldObject>();
	lights = std::vector<Light>();

	WorldObject floorObject = WorldObject();
	floorObject.objModel = worldFloor.GetObjModel();
	floorObject.location = glm::vec3(0,-1,0);
	floorObject.scale = glm::vec3(1, 1, 1);
	floorObject.name = "Floor";

	worldModels.push_back(floorObject);

	teleporters[0] = Teleporter();
	teleporters[1] = Teleporter();
}

void Space::Grow()
{
	VRScale *= 0.99f;
}

void Space::Shrink()
{
	if (VRScale < MaxScale)
		VRScale *= 1.01f;
}

Space::~Space()
{
}