#pragma once

#include <GL\glew.h>
#include <string>

class Shader
{
public:
	Shader(std::string vsfile, std::string fsfile);
	void link(void);
	void use(void);
	void setUniform(const GLchar*);
	void bindAttribute(GLuint index, const GLchar * value);
	GLuint programId;
	GLuint Uniform;
	GLuint modelViewUniform;
private:
	void checkShaderErrors(GLuint shaderId);
};