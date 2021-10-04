#pragma once

#include <windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <string>
#include <vector>

class Texture
{
private:
	bool is_cubemap = false;
public:
	Texture(std::string);
	Texture(std::vector<std::string> filenames);
	Texture(int, uint16_t, uint16_t, const uint8_t*);
	bool isCubeMap() { return is_cubemap; };
	~Texture();

	GLuint textureId;
};

