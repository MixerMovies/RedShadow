#version 400 core

in vec3 texCoord;

uniform samplerCube cubemap;

void main()
{
	gl_FragColor = texture(cubemap, texCoord);
}