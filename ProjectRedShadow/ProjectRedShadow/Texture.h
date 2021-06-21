#pragma once

#include <windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <string>

class Texture
{
public:
	Texture(std::string);
	Texture(int, uint16_t, uint16_t, const uint8_t*);
	~Texture();

	GLuint textureId;
};

