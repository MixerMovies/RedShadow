#pragma once

#include "Player.h"
#include "Sound.h"
#include "ObjModel.h"

class City
{
public:
	Player player = Player();
	static City* Instance();
	Sound *music = nullptr;
	ObjModel *building;
	~City();
private:
	static City *instance;
	City();
};

