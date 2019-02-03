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
	Player player = Player();
	Sound *music = nullptr;
	std::vector<WorldObject> worldModels;
	Skybox skybox = Skybox(500, 500, 500, new ObjModel("models/Skyboxes/std-skybox.obj"));;
	std::vector<ObjModel*> previewModels;
	void NextModel();
	void PreviousModel();
	ObjModel* getCurrentModel();
	~Space();
private:
	int currentModel = 0;
};

