#version 400 core

uniform sampler2D s_texture;
varying vec2 texCoord;

void main()
{
	vec4 color = texture2D(s_texture, texCoord);
	float length = sqrt((texCoord.x - 0.5) * (texCoord.x - 0.5) + (texCoord.y - 0.5) * (texCoord.y - 0.5));
	if(length < 0.4)
	{
		gl_FragColor = color;
	}
	else
	{
		gl_FragColor = vec4(0, 0, 0, 0);
	}
}