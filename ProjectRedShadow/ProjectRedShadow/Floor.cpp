#define _USE_MATH_DEFINES

#include "Floor.h"
#include "FileLoader.h"

#include <math.h>

Floor::Floor(int size)
{
	_size = size;
	GenerateFloor();
};

void Floor::GenerateFloor()
{
	std::vector<float> vertices = std::vector<float>();
	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(glm::cos(M_PI_2 * i + M_PI_4) * _size);
		vertices.push_back(0);
		vertices.push_back(glm::sin(M_PI_2 * i + M_PI_4) * _size);
	}

	std::vector<float> normals = std::vector<float>();
	normals.push_back(0);
	normals.push_back(1);
	normals.push_back(0);

	std::vector<float> textureCoordinates = std::vector<float>();
	for (int i = 0; i < 4; i++)
	{
		textureCoordinates.push_back(glm::cos(M_PI_2 * i + M_PI_4));
		textureCoordinates.push_back(glm::sin(M_PI_2 * i + M_PI_4));
	}

	_objModel = new ObjModel(vertices, normals, textureCoordinates, , new Texture(FileLoader::getMainPath() + "\\Textures\\White.png") );
}