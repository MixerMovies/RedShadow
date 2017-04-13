#version 150
uniform sampler2D s_texture;
varying vec2 texCoord;

void main()
{
    float o = 32/2048.0;

    vec4 color = (texture2D(s_texture, texCoord) +
                texture2D(s_texture, texCoord + vec2(-o, -o)) +
                texture2D(s_texture, texCoord + vec2(-o, o)) +
                texture2D(s_texture, texCoord + vec2(o, o)) +
                texture2D(s_texture, texCoord + vec2(o, -o))) / 5.0;

	gl_FragColor = color;
}