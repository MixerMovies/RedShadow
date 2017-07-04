#version 330 core

attribute vec3 a_position;
out vec2 texCoordIn;

void main()
{
	texCoordIn = (vec2(a_position) + vec2(1, 1)) / 2.0;
	gl_Position = vec4(a_position,1);
}