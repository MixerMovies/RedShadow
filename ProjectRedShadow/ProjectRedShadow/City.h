#pragma once

#include "Player.h"

class City
{
public:
	Player player = Player();
	static City* Instance();
	~City();
private:
	static City *instance;
	City();
};

