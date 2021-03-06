#version 400 core

uniform sampler2D s_texture;
uniform sampler2D bump_map;
uniform int has_bump_map;
uniform float shininess;
uniform float alpha;
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
	vec4 texture = texture2D(s_texture, texCoord);
	if(texture.w * alpha < 0.1)
		discard;

    vec3 normalized = normalize(normal);

    if(has_bump_map == 1)
    {
        vec4 bumpmap = texture2D(bump_map, texCoord);
	    vec3 bump = vec3(2*bumpmap.x-1, 2*bumpmap.y-1, 2*bumpmap.z-1);
	    normalized = normalize(normalized+bump);
    }

	vec3 lightDirection = normalize(lightPosition - fragPos);
	vec3 viewDirection = normalize(viewPosition - fragPos);

	float diffuseRes = max(0.0, dot(normalized, lightDirection));
	vec3 finalDif = diffuse * diffuseRes;

	vec3 r = reflect(-lightDirection, normalized);
	float specularRes = pow(max(0.0, dot(r, viewDirection)), shininess);
	vec3 finalSpec = specular * specularRes;

	vec3 factor = ambient + finalDif + finalSpec;

	gl_FragColor = vec4(factor,alpha) * texture * lightColor;
}