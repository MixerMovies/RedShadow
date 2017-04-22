#version 150
uniform sampler2D s_texture;
uniform float time;
varying vec2 texCoord;

void main()
{
	mat3 rotationMatrix = mat3(
		cos(time), sin(time), 0,
		-sin(time), cos(time), 0,
		0, 0, 1.0
	);

	vec3 rotatedPosition = vec3(texCoord.x, texCoord.y, 0);
	rotatedPosition = rotationMatrix * rotatedPosition;
	gl_FragColor = texture2D(s_texture, vec2(rotatedPosition.x, rotatedPosition.y));
}