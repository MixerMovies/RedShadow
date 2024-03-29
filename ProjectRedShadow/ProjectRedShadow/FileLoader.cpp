#define _USE_MATH_DEFINES

#include "FileLoader.h"

#include <math.h>
#include <fstream>
#include <vector>
#include <sstream>

FileLoader::Category FileLoader::category;

FileLoader::FileLoader()
{
}

/// <summary>
/// Loads the map using the given location into the current space.
/// </summary>
/// <param name="mapLocation">the path to the location of the map</param>
/// <param name="space">the space to load the objects of the map into</param>
void FileLoader::loadMap(std::string mapLocation, Space* space)
{
	category = DEFAULT;
	std::ifstream stream;
	stream.open(std::string("maps/") + mapLocation + std::string(".map"));
	if (stream.is_open())
	{
		std::string element;
		while (std::getline(stream, element))
		{
			if (element.size() < 1 || element.at(0) == '#')
				continue;
			else if (element.at(0) == '!')
			{
				if (element == "!models") category = MODEL;
				else if (element == "!lights") category = LIGHT;
			}
			else if (category == MODEL)
				loadModel(space, element);
			else if (category == LIGHT)
				loadLight(space, element);
		}
	}
}

/// <summary>
/// Load a model into the current space.
/// </summary>
/// <param name="space">the space containing models</param>
/// <param name="element">a string containing info about the model to be loaded</param>
void FileLoader::loadModel(Space* space, std::string element)
{
	std::vector<std::string> data = std::vector<std::string>();
	std::stringstream sstream(element);
	while (std::getline(sstream, element, '|'))
	{
		data.push_back(element);
	}
	WorldObject worldObject;
	worldObject.objModel = new ObjModel(data[0]);
	worldObject.name = data[1];
	std::stringstream vstream(data[2]);
	int i = 0;
	while (std::getline(vstream, element, ',') && i < 3)
	{
		worldObject.location[i] = stof(element);
		i++;
	}
	std::stringstream vstream2(data[3]);
	i = 0;
	while (std::getline(vstream2, element, ',') && i < 3)
	{
		worldObject.rotation[i] = stof(element) / 180.0f * M_PI;
		i++;
	}
	std::stringstream vstream3(data[4]);
	i = 0;
	while (std::getline(vstream3, element, ',') && i < 3)
	{
		worldObject.scale[i] = stof(element);
		i++;
	}
	space->worldModels.push_back(worldObject);
}

/// <summary>
/// Loads the lights for the current scene
/// </summary>
/// <param name="space">The space to add new lights to</param>
/// <param name="element">a string containing info about the light that needs to be added</param>
void FileLoader::loadLight(Space* space, std::string element)
{
	std::vector<std::string> data = std::vector<std::string>();
	std::stringstream sstream(element);
	while (std::getline(sstream, element, '|'))
	{
		data.push_back(element);
	}
	Light light;
	std::stringstream vstream(data[0]);
	int i = 0;
	while (std::getline(vstream, element, ',') && i < 3)
	{
		light.position[i] = stof(element);
		i++;
	}
	std::stringstream vstream2(data[1]);
	i = 0;
	while (std::getline(vstream2, element, ',') && i < 3)
	{
		light.color[i] = stof(element) / 255.0f;
		i++;
	}
	space->lights.push_back(light);
}

FileLoader::~FileLoader()
{
}
