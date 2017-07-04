#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
in vec3 a_position;
in vec2 a_texcoord;
in vec3 a_color;
in vec3 a_normal;
out vec2 texCoordIn;
out vec3 colorIn;
out vec3 normalIn;

void main()
{
	colorIn = a_color;
	normalIn = a_normal;
	texCoordIn = a_texcoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}