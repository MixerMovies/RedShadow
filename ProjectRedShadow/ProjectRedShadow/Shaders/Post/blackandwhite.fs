#version 150
uniform sampler2D s_texture;
varying vec2 texCoord;

void main()
{
	vec4 color = texture2D(s_texture, texCoord);
	float value = (color.x + color.y + color.z) / 3.0;
	gl_FragColor = vec4(value);
}