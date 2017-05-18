attribute vec3 a_position;
attribute vec3 a_color;
attribute vec2 a_texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
varying vec3 color;
varying vec3 pos;
varying vec2 texCoord;

void main()
{
	color = a_color;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
	pos = a_position;
	texCoord = a_texcoord;
}