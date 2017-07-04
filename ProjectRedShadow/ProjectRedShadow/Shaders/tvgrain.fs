#version 330 core

varying vec3 color;

float random (in vec2 st) { 
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

void main()
{
	gl_FragColor = vec4(random(color.xy),random(color.xy),random(color.xy), 1.0);
}