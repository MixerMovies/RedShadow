#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "stb_image.h"

Texture::Texture(std::string filename)
{
	int width, height, comp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &comp, 4);
	if (!data)
	{
		printf("Failed to load: %s\n", stbi_failure_reason());
		throw "Failed to load";
	}
	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &textureId);
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, textureId);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

Texture::Texture(int textureID, uint16_t height, uint16_t width, const uint8_t* data)
{
	textureId = textureID;
	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &textureId);
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, textureId);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &textureId);
}
