#pragma once

#include "ObjModel.h"

class Floor
{
private:
	int _size = 0;
	ObjModel* _objModel;
	void GenerateFloor();
public:
	Floor(int size);
	int GetSize() { return _size; }
};