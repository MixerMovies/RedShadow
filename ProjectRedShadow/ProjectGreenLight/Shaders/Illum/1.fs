#version 400 core

uniform sampler2D s_texture;
uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 diffuse;
uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec4 lightColor;
varying vec3 normal;
varying vec2 texCoord;
varying vec3 fragPos;

void main()
{
	vec4 textureFragment = texture(s_texture, texCoord);

    vec3 normalized = normalize(normal);

	vec3 lightDirection = normalize(lightPosition - fragPos);

	float diffuseRes = max(0.0, dot(normalized, lightDirection));
	vec3 finalDif = diffuse * diffuseRes;

	vec3 factor = ambient + finalDif;

	gl_FragColor = vec4(factor,1) * textureFragment * lightColor;
}