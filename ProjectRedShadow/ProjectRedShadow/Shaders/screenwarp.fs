#version 330 core

uniform sampler2D s_texture;
uniform float time;
varying vec2 texCoord;

void main()
{
	float length = sqrt(((texCoord.x - 0.5) * (texCoord.x - 0.5)) + ((texCoord.y - 0.5) * (texCoord.y - 0.5)));

	mat3 rotationMatrix = mat3(
		cos(time * length), -sin(time * length), 0,
		sin(time * length), cos(time * length), 0,
		0, 0, 1.0
	);

	vec3 rotatedPosition = vec3(texCoord.x - 0.5, texCoord.y - 0.5, 0);
	rotatedPosition = rotationMatrix * rotatedPosition;
	gl_FragColor = texture2D(s_texture, vec2(rotatedPosition.x + 0.5, rotatedPosition.y + 0.5));
}