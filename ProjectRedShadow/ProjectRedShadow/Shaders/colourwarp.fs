#version 150
uniform sampler2D s_texture;
uniform float time;
varying vec2 texCoord;

void main()
{
	float length = sqrt(((texCoord.x - 0.5) * (texCoord.x - 0.5)) + ((texCoord.y - 0.5) * (texCoord.y - 0.5)));

	vec3 translatedPosition = vec3(texCoord.x - 0.5, texCoord.y - 0.5, 0);
	gl_FragColor = texture2D(s_texture, vec2((translatedPosition.x * length) + 0.5, (translatedPosition.y * length) + 0.5));
}