#version 400 core
uniform sampler2D s_texture;
varying vec2 texCoord;

void main()
{
	vec4 color = texture2D(s_texture, texCoord);
	gl_FragColor = vec4(1.0 - color.x, 1.0 - color.y, 1.0 - color.z, 1.0);
}