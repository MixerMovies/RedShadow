uniform sampler2D s_texture;
uniform float time;
varying vec2 texCoord;

void main()
{
	gl_FragColor = texture2D(s_texture, texCoord + vec2(time,0));
}