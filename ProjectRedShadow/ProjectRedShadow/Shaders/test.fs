#version 330 core

uniform sampler2D s_texture;
in vec2 texCoord;

void main()
{
    gl_FragColor = texture2D(s_texture, texCoord);
}