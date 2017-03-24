#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <string>

class Texture
{
public:
	Texture(std::string);
	~Texture();

	GLuint textureId;
};

