#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec2 texCoordIn[];
in vec3 colorIn[];
in vec3 normalIn[];
uniform float time;
out vec2 texCoord;
out vec3 color;
out vec3 normal;

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 20.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0);
}

void main() {
	vec3 expNormal = getNormal();
    
    gl_Position = explode(gl_in[0].gl_Position, expNormal);
	texCoord = texCoordIn[0];
	color = colorIn[0];
	normal = normalIn[0];
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, expNormal);
	texCoord = texCoordIn[1];
	color = colorIn[1];
	normal = normalIn[1];
    EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, expNormal);
	texCoord = texCoordIn[2];
	color = colorIn[2];
	normal = normalIn[2];
    EmitVertex();
    
    EndPrimitive();
}  