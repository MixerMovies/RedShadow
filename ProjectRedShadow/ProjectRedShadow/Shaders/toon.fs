#version 330 core

uniform sampler2D s_texture;
varying vec3 normal;
varying vec2 texCoord;

void main()
{
	vec3 lightDirection = normalize(vec3(1,1,1));
	vec3 viewDirection = vec3(0,0,1);
	float shininess = 10.0;

	float ambient = 0.2;
	float diffuse = 0.8 * dot(normal, lightDirection);

	vec3 r = reflect(-lightDirection, normal);

	float specular = pow(max(0.0, dot(r, viewDirection)), shininess);

	float factor = ambient + diffuse + specular;
	
	vec4 normalColor = vec4(factor,factor,factor,1.0) * texture2D(s_texture, texCoord);
	gl_FragColor = vec4(round(normalColor[0] * 5) / 5, round(normalColor[1] * 5) / 5, round(normalColor[2] * 5) / 5, 1.0);
}