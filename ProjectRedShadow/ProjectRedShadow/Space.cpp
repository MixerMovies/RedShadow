#include "Space.h"


Space::Space()
{
	worldModels = std::vector<WorldObject>();
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