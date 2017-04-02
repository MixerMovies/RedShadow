varying vec3 normal;

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
	gl_FragColor = vec4(factor,factor,factor,1.0);
}