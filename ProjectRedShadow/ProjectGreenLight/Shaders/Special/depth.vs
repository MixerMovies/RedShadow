#version 400 core

uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;
in vec3 a_position;
out vec3 fragPosIn;

void main()
{
	gl_Position = lightSpaceMatrix * modelMatrix * vec4(a_position,1);
}