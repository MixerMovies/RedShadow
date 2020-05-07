#pragma once

#include "Skybox.h"
#include "Player.h"
#include "Sound.h"
#include "ObjModel.h"
#include "Teleporter.h"

class Space
{
public:
	struct WorldObject
	{
		ObjModel* objModel;
		std::string name;
		glm::vec3 location;
		glm::vec3 scale;
	};
	Space();
	float MaxScale = 1.0f;
	Player player = Player();
	Sound *music = nullptr;
	std::vector<WorldObject> worldModels;
	Teleporter teleporter = Teleporter();
	Skybox skybox = Skybox(500, 500, 500, new ObjModel("models/Skyboxes/std-skybox.obj"));
	glm::vec3 lightPosition = glm::vec3(1, 1, 0);
	glm::vec3 lightColor = glm::vec3(1, 1, 1);
	float VRScale = MaxScale;
	void Grow();
	void Shrink();
	~Space();
private:
	int currentModel = 0;
};

