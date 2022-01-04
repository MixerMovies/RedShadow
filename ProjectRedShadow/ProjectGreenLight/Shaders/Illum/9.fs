#version 400 core

uniform sampler2D s_texture;
uniform float shininess;
uniform float alpha;
uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 diffuse;
uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec4 lightColor;
uniform samplerCube skybox;

varying vec3 normal;
varying vec2 texCoord;
varying vec3 fragPos;

void main()
{
    float ratio = 0.658;
    vec3 I = normalize(fragPos - viewPosition);
    vec3 R = refract(I, normalize(normal), ratio);
    gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
}