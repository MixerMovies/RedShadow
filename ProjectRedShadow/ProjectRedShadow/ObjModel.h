#pragma once

#include "framework.h"

#include <vector>

#include <windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "MaterialInfo.h"

class RED_EXPORT ObjModel
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

	struct ShaderMatrices
	{
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;
		
		glm::mat3 normalMatrix;

		glm::vec3 viewPosition;
		glm::vec3 lightPosition;
		glm::vec3 lightColour;
	};

	ObjModel(std::string filename);
	ObjModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> textureCoordinats, std::vector<uint16_t> indices, Texture* texture, Shader* shader);
	~ObjModel(void);

	void draw(ShaderMatrices matrices, Shader* shader = nullptr);
};

