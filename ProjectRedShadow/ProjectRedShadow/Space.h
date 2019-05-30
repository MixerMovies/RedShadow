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
	ObjModel* prototypeController = new ObjModel("models/ship/ShipA_OBJ.obj");
	std::vector<ObjModel*> previewModels;
	float VRScale = MaxScale;
	void NextModel();
	void PreviousModel();
	void Grow();
	void Shrink();
	ObjModel* getCurrentModel();
	~Space();
private:
	int currentModel = 0;
};

