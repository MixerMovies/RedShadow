#pragma once

#include <GL\glew.h>

class Shader
{
	Shader(std::string vsfile, std::string fsfile);
	void checkShaderErrors(GLuint shaderId);
	void link(void);
	void use(void);
	void setUniform(GLuint, const GLchar*);
	virtual void bindAttributes() = 0;
	GLuint programId;
	GLuint Uniform;
};