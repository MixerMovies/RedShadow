#include "Space.h"


Space::Space()
{
	worldModels = std::vector<WorldObject>();

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