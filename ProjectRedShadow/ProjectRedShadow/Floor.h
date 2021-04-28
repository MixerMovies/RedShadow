#pragma once

#include "WorldObject.h"

class WorldFloor
{
private:
	int _size = 0;
	WorldObject _wObject;
	ObjModel* _generatedModel;
	void GenerateFloor();
public:
	WorldFloor(int size);
	int GetSize() { return _size; };
	WorldObject GetWorldObject() { return _wObject; };
};