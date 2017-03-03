#pragma once
#include <string>

class Texture
{
	unsigned int textureID;
public:
	Texture(const std::string &filename);
	void bind();
	int height;
	int width;
};