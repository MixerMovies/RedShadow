attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
uniform mat4 modelViewProjectionMatrix;
varying vec2 texCoord;

void main()
{
	texCoord = a_texcoord;
	gl_Position = modelViewProjectionMatrix * vec4(a_position,1);
}