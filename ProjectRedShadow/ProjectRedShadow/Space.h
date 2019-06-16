#pragma once

#include "Skybox.h"
#include "Player.h"
#include "Sound.h"
#include "ObjModel.h"

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
	float MaxScale = 0.2f;
	Player player = Player();
	Sound *music = nullptr;
	std::vector<WorldObject> worldModels;
	Skybox skybox = Skybox(500, 500, 500, new ObjModel("models/Skyboxes/std-skybox.obj"));
	glm::vec4 lightPosition = glm::vec4(1, 1, 0, 1);
	float VRScale = MaxScale;
	void Grow();
	void Shrink();
	~Space();
private:
	int currentModel = 0;
};

