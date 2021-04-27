#define _USE_MATH_DEFINES

#include "Floor.h"
#include "FileLoader.h"

#include <math.h>

WorldFloor::WorldFloor(int size)
{
	_size = size;
	GenerateFloor();
};

void WorldFloor::GenerateFloor()
{
	std::vector<float> vertices = std::vector<float>();
	for (unsigned int i = 0; i < 4; i++)
	{
		vertices.push_back(glm::cos(M_PI_2 * i + M_PI_4) * _size);
		vertices.push_back(0);
		vertices.push_back(glm::sin(M_PI_2 * i + M_PI_4) * _size);
	}

	std::vector<float> normals = std::vector<float>();
	for (unsigned int i = 0; i < 4; i++)
	{
		normals.push_back(0);
		normals.push_back(1);
		normals.push_back(0);
	}

	std::vector<float> textureCoordinates = std::vector<float>();
	for (unsigned int i = 0; i < 4; i++)
	{
		textureCoordinates.push_back(glm::cos(M_PI_2 * i + M_PI_4));
		textureCoordinates.push_back(glm::sin(M_PI_2 * i + M_PI_4));
	}

	std::vector<uint16_t> indices = std::vector<uint16_t>();
	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(1);

	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(1);

	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(1);

	_generatedModel = new ObjModel(vertices, normals, textureCoordinates, indices, new Texture(FileLoader::getMainPath() + "\\Textures\\White.png") );
	_objModel = new ObjModel("models/floor/floor.obj");
}