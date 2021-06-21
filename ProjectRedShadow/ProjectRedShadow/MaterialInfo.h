#pragma once

#include <vector>

#include "Texture.h"

class MaterialInfo
{
public:
	MaterialInfo();
	static void loadMaterialFile(std::vector<MaterialInfo*>& materials, std::string fileName, std::string dirName);
	std::string name;
	Texture* texture;
	Texture* bumpMap;
	float ambient[3] = { 0, 0, 0 };
	float diffuse[3] = { 0, 0, 0 };
	float specular[3] = { 0, 0, 0 };
	float shininess = 10.0f;
	float alpha = 1.0f;
	bool hasTexture;
};
