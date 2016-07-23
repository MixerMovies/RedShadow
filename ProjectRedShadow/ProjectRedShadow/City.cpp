#include "City.h"



City::City()
{
}

City* City::instance = NULL;

City * City::Instance()
{
	if (!instance)
	{
		instance = new City();
	}
	return instance;
}

City::~City()
{
}
