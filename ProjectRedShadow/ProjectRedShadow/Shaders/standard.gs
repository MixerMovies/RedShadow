#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec2 texCoordIn[];
in vec3 colorIn[];
in vec3 normalIn[];
in vec3 fragPosIn[];
out vec2 texCoord;
out vec3 color;
out vec3 normal;
out vec3 fragPos;

void main() {    
    gl_Position = gl_in[0].gl_Position;
	texCoord = texCoordIn[0];
	color = colorIn[0];
	normal = normalIn[0];
	fragPos = fragPosIn[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
	texCoord = texCoordIn[1];
	color = colorIn[1];
	normal = normalIn[1];
	fragPos = fragPosIn[1];
    EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	texCoord = texCoordIn[2];
	color = colorIn[2];
	normal = normalIn[2];
	fragPos = fragPosIn[2];
    EmitVertex();
    
    EndPrimitive();
}  