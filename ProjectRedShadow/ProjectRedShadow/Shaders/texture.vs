#version 330 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
in vec3 a_position;
in vec2 a_texcoord;
out vec2 texCoordIn;

void main()
{
	texCoordIn = a_texcoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}