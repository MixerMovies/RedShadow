#version 400 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
in vec3 a_position;
out vec3 texCoordIn;

void main()
{
	texCoordIn = a_position;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}