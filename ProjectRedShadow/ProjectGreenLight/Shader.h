#pragma once

#include <GL\glew.h>
#include <string>
#include <map>

class Shader
{
public:
	Shader(std::string vsfile, std::string fsfile, std::string gsfile);
	void link(void);
	void use(void);
	int getUniformLocation(GLchar* name);
	void bindAttribute(GLuint index, const GLchar * value);
	GLuint programId;
private:
	void checkShaderErrors(GLuint shaderId);
	std::map<GLchar*, GLuint> uniforms;
};