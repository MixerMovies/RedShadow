#version 330 core

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
//attribute vec3 a_color;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec3 normalIn;
out vec2 texCoordIn;
out vec3 colorIn;

void main()
{
	texCoordIn = a_texcoord;
	normalIn = normalMatrix * a_normal;
	colorIn = vec3(0,0,0);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}