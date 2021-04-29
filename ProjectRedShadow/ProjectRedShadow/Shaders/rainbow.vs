#version 400 core

attribute vec3 a_position;
attribute vec3 a_color;
attribute vec2 a_texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 normalIn;
out vec2 texCoordIn;
out vec3 colorIn;
out vec3 fragPosIn;

void main()
{
	colorIn = a_color;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
	fragPosIn = a_position;
	texCoordIn = a_texcoord;
}