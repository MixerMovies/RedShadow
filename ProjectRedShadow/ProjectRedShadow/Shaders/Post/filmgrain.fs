#version 150
uniform sampler2D s_texture;
uniform float time;
varying vec2 texCoord;

float random (in vec2 st) { 
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

void main()
{
	vec2 noiseseed = vec2(texCoord.x + time, texCoord.y + time);
    vec4 color = texture2D(s_texture, texCoord) - random(noiseseed) * 0.4 + 0.2;
	gl_FragColor = color;
}