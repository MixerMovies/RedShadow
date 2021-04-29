#version 400 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
in vec3 a_position;
in vec2 a_texcoord;
in vec3 a_normal;
out vec2 texCoordIn;
out vec3 colorIn;
out vec3 normalIn;
out vec3 fragPosIn;

void main()
{
	colorIn = vec3(0,0,0);
	normalIn = normalMatrix * a_normal;
	texCoordIn = a_texcoord;
	fragPosIn = vec3(modelMatrix * vec4(a_position, 1));
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}