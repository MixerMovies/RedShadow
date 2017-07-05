#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec2 texCoordIn[];
in vec3 colorIn[];
in vec3 normalIn[];
uniform float time;
out vec2 texCoord;
out vec3 color;
out vec3 normal;

vec4 getCenter(vec4 position)
{
	float x = (gl_in[0].gl_Position.x + gl_in[1].gl_Position.x + gl_in[2].gl_Position.x) / 3;
	float y = (gl_in[0].gl_Position.y + gl_in[1].gl_Position.y + gl_in[2].gl_Position.y) / 3;
	float z = (gl_in[0].gl_Position.z + gl_in[1].gl_Position.z + gl_in[2].gl_Position.z) / 3;
	float w = (gl_in[0].gl_Position.w + gl_in[1].gl_Position.w + gl_in[2].gl_Position.w) / 3;
	return vec4((x * (1 + sin(time + 3.14)) + position.x * (1 + sin(time))) / (1 + sin(time)),
	 (y * (1 + sin(time + 3.14)) + position.y * (1 + sin(time))) / (1 + sin(time)),
	 (z * (1 + sin(time + 3.14)) + position.z * (1 + sin(time))) / (1 + sin(time)),
	 (w * (1 + sin(time + 3.14)) + position.w * (1 + sin(time))) / (1 + sin(time)));
}

vec4 implode(vec4 position, vec3 normal)
{
	float magnitude = 10.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(normal, 0.0);
}

void main() {
    gl_Position = getCenter(gl_in[0].gl_Position);
	texCoord = texCoordIn[0];
	color = colorIn[0];
	normal = normalIn[0];
    EmitVertex();

    gl_Position = getCenter(gl_in[1].gl_Position);
	texCoord = texCoordIn[1];
	color = colorIn[1];
	normal = normalIn[1];
    EmitVertex();

	gl_Position = getCenter(gl_in[2].gl_Position);
	texCoord = texCoordIn[2];
	color = colorIn[2];
	normal = normalIn[2];
    EmitVertex();
    
    EndPrimitive();
}  