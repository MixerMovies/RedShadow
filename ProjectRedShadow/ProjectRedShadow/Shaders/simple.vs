attribute vec3 a_position;
attribute vec3 a_color;
uniform mat4 modelViewProjectionMatrix;
varying vec3 color;

void main()
{
	color = a_color;
	gl_Position = modelViewProjectionMatrix * vec4(a_position,1);
}