attribute vec3 a_position;
attribute vec3 a_normal;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

varying vec3 normal;

void main()
{
	normal = normalMatrix * vec4(a_normal,1);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}