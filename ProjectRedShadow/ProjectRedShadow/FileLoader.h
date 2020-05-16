#pragma once

#include <experimental/filesystem>
#include <string>
#include "Space.h"

class FileLoader
{
public:
	FileLoader();
	static std::string getMainPath() { return std::experimental::filesystem::current_path().string(); }
	static void loadMap(std::string, Space*);
	~FileLoader();
};

