#version 400 core

varying vec3 color;
uniform float time;

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

float noise (in vec2 st) {
	vec2 i = floor(st);
	vec2 f = fract(st);

	vec2 u = f*f*(3.0-2.0*f);

	return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ), 
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

void main()
{
	vec2 st = gl_FragCoord.xy/vec2(1920,1080);
	st.x *= 1920/1080;
	vec2 pos = vec2(st*(time));

	vec3 n = vec3(1.0 - noise(pos)*.5+.5, noise(pos)*.5+.5, 0);

	gl_FragColor = vec4(n,1.0);
}
