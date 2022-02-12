#pragma once

#include <filesystem>
#include <string>
//#include "Space.h"

class FileLoader
{
private:
	enum Category { MODEL, LIGHT, DEFAULT };
	static Category category;
public:
	FileLoader();
	static std::string getMainPath() { return std::filesystem::current_path().string(); }
	//static void loadMap(std::string, Space*);
	~FileLoader();
private:
	//static void loadModel(Space*, std::string);
	//static void loadLight(Space*, std::string);
};
