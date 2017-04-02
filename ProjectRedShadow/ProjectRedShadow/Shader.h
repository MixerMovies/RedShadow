#pragma once

#include <GL\glew.h>
#include <string>

class Shader
{
public:
	Shader(std::string vsfile, std::string fsfile);
	void link(void);
	void use(void);
	int getUniformLocation(const std::string & name);
	void bindAttribute(GLuint index, const GLchar * value);
	GLuint programId;
	GLuint Uniform;
private:
	void checkShaderErrors(GLuint shaderId);
};