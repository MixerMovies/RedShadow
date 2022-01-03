#version 400 core

uniform sampler2D s_texture;
uniform vec3 diffuse;
uniform vec3 ambient;
in vec2 texCoord;

void main()
{
    gl_FragColor = texture(s_texture, texCoord) * vec4(diffuse, 1);
}