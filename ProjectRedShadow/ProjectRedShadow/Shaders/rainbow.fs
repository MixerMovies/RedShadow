uniform sampler2D s_texture;
varying vec3 color;
varying vec3 pos;
varying vec2 texCoord;
uniform float time;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;

void main()
{
	float scale = 1.5;

	float time1 = mod(time/1.0f,scale);
	if (time1 > scale/3)
	time1 = scale/3*2 - time1;

	float r = mod((sin(time1*pos.x*pos.z)),1.0f);
	float g =  mod((cos(time1*4*pos.y*500)),1.0f);
	float b =  mod((sin(time1/3*pos.z*pos.y*500)),1.0f);
	gl_FragColor.r = r;
	gl_FragColor.b = b;
	gl_FragColor.g = g;
}