#pragma once

#include <string>
#include "Space.h"

class FileLoader
{
public:
	FileLoader();
	static void loadMap(std::string, Space*);
	~FileLoader();
};

