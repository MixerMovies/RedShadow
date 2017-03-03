#include "Space.h"



Space::Space()
{
}

Space* Space::instance = NULL;

Space * Space::Instance()
{
	if (!instance)
	{
		instance = new Space();
	}
	return instance;
}

Space::~Space()
{
}
