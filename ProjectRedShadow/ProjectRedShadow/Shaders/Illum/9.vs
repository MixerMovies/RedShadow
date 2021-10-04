#version 400 core

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec3 normalIn;
out vec2 texCoordIn;
out vec3 colorIn;
out vec3 fragPosIn;

void main()
{
	texCoordIn = a_texcoord;
	normalIn = mat3(transpose(inverse(modelMatrix))) * a_normal;
	colorIn = vec3(0,0,0);
	fragPosIn = vec3(modelMatrix * vec4(a_position, 1));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}