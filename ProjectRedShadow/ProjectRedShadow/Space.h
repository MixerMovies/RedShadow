#pragma once

#include "Skybox.h"
#include "Player.h"
#include "Sound.h"
#include "Teleporter.h"
#include "Light.h"
#include "Floor.h"
#include "WorldObject.h"

class Space
{
public:
	Space();
	float MaxScale = 1.0f;
	Player player = Player();
	Sound *music = nullptr;
	std::vector<WorldObject> worldModels;
	std::vector<Light> lights;
	Teleporter teleporters[2];
	WorldFloor worldFloor = WorldFloor(500);
	Skybox skybox = Skybox(0, 0, 0, 500, new ObjModel("models/Projects/Skyboxes/std-skybox.obj"));
	float VRScale = MaxScale;
	void Grow();
	void Shrink();
	~Space();
};
