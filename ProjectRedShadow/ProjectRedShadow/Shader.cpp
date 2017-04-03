#include "Shader.h"
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Shader::Shader(std::string vsfile, std::string fsfile)
{
	std::cout << "loading: " << vsfile << " and " << fsfile << std::endl;
	std::ifstream vertexShaderFile(vsfile);
	std::string vertexShaderData((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
	const char* cvertexShaderData = vertexShaderData.c_str();

	std::ifstream fragShaderFile(fsfile);
	std::string fragShaderData((std::istreambuf_iterator<char>(fragShaderFile)), std::istreambuf_iterator<char>());
	const char* cfragShaderData = fragShaderData.c_str();

	programId = glCreateProgram();								// maak een shaderprogramma aan

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);			// maak vertex shader aan
	glShaderSource(vertexId, 1, &cvertexShaderData, NULL);		// laat opengl de shader uit de variabele 'vertexShader' halen
	glCompileShader(vertexId);									// compileer de shader
	checkShaderErrors(vertexId);								// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, vertexId);						// hang de shader aan het shaderprogramma

	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);		// maak fragment shader aan
	glShaderSource(fragmentId, 1, &cfragShaderData, NULL);		// laat opengl de shader uit de variabele 'fragmentShader' halen
	glCompileShader(fragmentId);								// compileer de shader
	checkShaderErrors(fragmentId);								// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, fragmentId);						// hang de shader aan het shaderprogramma

	bindAttribute(0, "a_position");
	bindAttribute(1, "a_color");
	bindAttribute(2, "a_texcoord");
	bindAttribute(3, "a_normal");
}

void Shader::checkShaderErrors(GLuint shaderId)
{
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);					//kijk of het compileren is gelukt
	if (status == GL_FALSE)
	{
		int length, charsWritten;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);				//haal de lengte van de foutmelding op
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		glGetShaderInfoLog(shaderId, length, &charsWritten, infolog);		//en haal de foutmelding zelf op
		std::cout << "Error compiling shader:\n" << infolog << std::endl;
		delete[] infolog;
	}
}

void Shader::link()
{
	glLinkProgram(programId);								// link het programma, zorg dat alle attributes en varying gelinked zijn
}

void Shader::use()
{
	glUseProgram(programId);								// Zet dit als actieve programma
}

int Shader::getUniformLocation(const std::string &name)
{
	auto it = uniforms.find(name);
	if (it != uniforms.end())
		return it->second;
	GLuint location = glGetUniformLocation(programId, name.c_str());
	uniforms[name] = location;
	return location;
}

void Shader::bindAttribute(GLuint index, const GLchar* value)
{
	glBindAttribLocation(programId, index, value);
}
