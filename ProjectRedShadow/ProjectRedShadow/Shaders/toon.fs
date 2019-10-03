#version 330 core

uniform sampler2D s_texture;
uniform sampler2D bump_map;
uniform float shininess;
uniform float ambient;
uniform float intensity;
uniform vec3 diffuse;
uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec4 lightColor;
varying vec3 normal;
varying vec2 texCoord;
varying vec3 fragPos;

void main()
{
    vec3 normalized = normalize(normal);

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 viewDirection = normalize(viewPosition - fragPos);

	float diffuseRes = 0.8 * dot(normalized, lightDirection);

	vec3 r = reflect(-lightDirection, normalized);

	float specular = pow(max(0.0, dot(r, viewDirection)), shininess) * intensity;

	float factor = ambient + diffuseRes + specular;
	
	vec4 normalColor = vec4(factor,factor,factor,1.0) * texture2D(s_texture, texCoord) * vec4(diffuse, 1) * lightColor;
	gl_FragColor = vec4(round(normalColor[0] * 5) / 5, round(normalColor[1] * 5) / 5, round(normalColor[2] * 5) / 5, 1.0);
}