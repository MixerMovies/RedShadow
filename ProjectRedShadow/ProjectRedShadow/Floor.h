#pragma once

#include "ObjModel.h"

class WorldFloor
{
private:
	int _size = 0;
	ObjModel* _objModel;
	void GenerateFloor();
public:
	WorldFloor(int size);
	int GetSize() { return _size; };
	ObjModel* GetObjModel() { return _objModel; };
};