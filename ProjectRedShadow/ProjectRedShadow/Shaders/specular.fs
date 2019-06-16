#version 330 core

uniform sampler2D s_texture;
uniform float ambient;
uniform float shininess;
uniform float alpha;
uniform vec3 viewPosition;
uniform vec3 lightPosition;
varying vec3 normal;
varying vec2 texCoord;
varying vec3 fragPos;

void main()
{
    vec3 normalized = normalize(normal);

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 viewDirection = normalize(viewPosition - fragPos);

	float diffuse = 0.8 * dot(normalized, lightDirection);

	vec3 r = reflect(-lightDirection, normalized);

	float specular = pow(max(0.0, dot(r, viewDirection)), shininess);

	float factor = ambient + diffuse + specular;
	gl_FragColor = vec4(factor,factor,factor,alpha) * texture2D(s_texture, texCoord);
}