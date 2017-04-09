#include "Space.h"



Space::Space()
{
}

void Space::NextModel()
{
	currentModel = (currentModel + 1) % previewModels.size();
}

void Space::PreviousModel()
{
	currentModel = (currentModel + previewModels.size() - 1) % previewModels.size();
}

ObjModel * Space::getCurrentModel()
{
	return previewModels[currentModel];
}

Space::~Space()
{
}