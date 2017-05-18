#version 150
uniform sampler2D s_texture;
uniform float time;
varying vec2 texCoord;

void main()
{
	vec4 texture = texture2D(s_texture, texCoord);
	gl_FragColor = vec4(mod(texture.x + mod(time + 2, 6) / 5,1), mod(texture.y + mod(time + 4, 6) / 5,1), mod(texture.z + mod(time, 6) / 5,1), 1.0);
}