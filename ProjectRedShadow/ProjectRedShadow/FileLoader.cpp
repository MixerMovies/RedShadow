#include "FileLoader.h"
#include <fstream>
#include <vector>
#include <sstream>

FileLoader::FileLoader()
{
}

void FileLoader::loadMap(std::string mapLocation, Space* space)
{
	std::ifstream stream;
	stream.open(std::string("maps/") + mapLocation + std::string(".map"));
	if (stream.is_open())
	{
		std::string element;
		while (std::getline(stream, element))
		{
			std::vector<std::string> data = std::vector<std::string>();
			std::stringstream sstream(element);
			while (std::getline(sstream, element, '-'))
			{
				data.push_back(element);
			}
			std::string location = data[2];
			Space::WorldObject worldObject;
			worldObject.objModel = new ObjModel(data[0]);
			worldObject.name = data[1];
			std::stringstream vstream(data[2]);
			int i = 0;
			while (std::getline(vstream, element, ','))
			{
				worldObject.vector[i] = stof(element);
				i++;
			}
			space->worldModels.push_back(worldObject);
		}
	}
}


FileLoader::~FileLoader()
{
}
