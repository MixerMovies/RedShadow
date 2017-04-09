#pragma once

#include "Player.h"
#include "Sound.h"
#include "ObjModel.h"

class Space
{
public:
	Space();
	Player player = Player();
	Sound *music = nullptr;
	ObjModel *building;
	std::vector<ObjModel*> previewModels;
	void NextModel();
	void PreviousModel();
	ObjModel* getCurrentModel();
	~Space();
private:
	int currentModel = 0;
};

