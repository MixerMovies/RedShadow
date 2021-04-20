#version 330 core

uniform sampler2D s_texture;
uniform vec3 diffuse;
uniform vec3 ambient;
in vec2 texCoord;

void main()
{
    gl_FragColor = texture2D(s_texture, texCoord) * (vec4(diffuse, 1) + vec4(ambient, 1));
}