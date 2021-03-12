#pragma once

#include "Skybox.h"
#include "Player.h"
#include "Sound.h"
#include "ObjModel.h"
#include "Teleporter.h"
#include "Light.h"
#include "Floor.h"

class Space
{
public:
	struct WorldObject
	{
		ObjModel* objModel = nullptr;
		std::string name;
		glm::vec3 location;
		glm::vec3 scale;
	};
	Space();
	float MaxScale = 1.0f;
	Player player = Player();
	Sound *music = nullptr;
	std::vector<WorldObject> worldModels;
	std::vector<Light> lights;
	Teleporter teleporters[2];
	WorldFloor worldFloor = WorldFloor(500000);
	//Skybox skybox = Skybox(500, 500, 500, new ObjModel("models/Skyboxes/std-skybox.obj"));
	float VRScale = MaxScale;
	void Grow();
	void Shrink();
	~Space();
private:
	int currentModel = 0;
};

