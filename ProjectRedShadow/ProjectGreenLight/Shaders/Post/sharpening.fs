#version 150
uniform sampler2D s_texture;
varying vec2 texCoord;

void main()
{
    float o = 8/2048.0;

    vec4 color = (texture2D(s_texture, texCoord) * 5 -
                texture2D(s_texture, texCoord + vec2(-o, -o)) -
                texture2D(s_texture, texCoord + vec2(-o, o)) -
                texture2D(s_texture, texCoord + vec2(o, o)) -
                texture2D(s_texture, texCoord + vec2(o, -o)));

	gl_FragColor = color;
}