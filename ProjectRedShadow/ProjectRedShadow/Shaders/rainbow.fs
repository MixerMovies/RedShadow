#version 330 core

varying vec3 fragPos;
uniform float time;

void main()
{
	float scale = 1.5;

	float time1 = mod(time/1.0f,scale);
	if (time1 > scale/3)
	time1 = scale/3*2 - time1;

	float r = mod((sin(time1*fragPos.x*fragPos.z)),1.0f);
	float g = mod((cos(time1*4*fragPos.y*500)),1.0f);
	float b = mod((sin(time1/3*fragPos.z*fragPos.y*500)),1.0f);
	gl_FragColor.r = r;
	gl_FragColor.b = b;
	gl_FragColor.g = g;
}