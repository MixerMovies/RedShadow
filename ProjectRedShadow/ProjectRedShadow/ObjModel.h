#pragma once

#include <string>
#include <vector>

#include <windows.h>
#include <gl/GL.h>

#include "Texture.h"
#include "Shader.h"

class Texture;

class ObjModel
{
private:
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		Texture* texture;
		Texture* bumpMap;
		float ambient[3] = {0, 0, 0};
		float shininess = 10;
		float alpha = 1.0f;
		bool hasTexture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int start;
		int end;
		int materialIndex;
	};

	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;
	GLuint _vertexArray;
	GLsizei size = 0;

	void loadMaterialFile(std::string fileName, std::string dirName);
public:
	ObjModel(std::string filename);
	ObjModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> textureCoordinats, std::vector<uint16_t> indices, Texture* texture);
	~ObjModel(void);

	void draw(Shader* shader);
};

