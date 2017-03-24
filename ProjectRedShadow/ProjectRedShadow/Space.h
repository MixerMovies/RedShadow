#pragma once

#include "Player.h"
#include "Sound.h"
#include "ObjModel.h"

class Space
{
public:
	Player player = Player();
	static Space* Instance();
	Sound *music = nullptr;
	ObjModel *building;
	ObjModel *Spaceship;
	~Space();
private:
	static Space *instance;
	Space();
};

