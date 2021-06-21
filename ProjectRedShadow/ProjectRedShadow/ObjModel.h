#pragma once

#include <vector>

#include <windows.h>
#include <gl/glew.h>
#include <gl/GL.h>

#include "Shader.h"
#include "Texture.h"
#include "MaterialInfo.h"

class ObjModel
{
private:

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

public:
	ObjModel(std::string filename);
	ObjModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> textureCoordinats, std::vector<uint16_t> indices, Texture* texture);
	~ObjModel(void);

	void draw(Shader* shader);
};

