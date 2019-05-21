#include "Space.h"


Space::Space()
{
	worldModels = std::vector<WorldObject>();
}

void Space::NextModel()
{
	currentModel = (currentModel + 1) % previewModels.size();
}

void Space::PreviousModel()
{
	currentModel = (currentModel + previewModels.size() - 1) % previewModels.size();
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

ObjModel * Space::getCurrentModel()
{
	return previewModels[currentModel];
}

Space::~Space()
{
}