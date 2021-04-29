#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in vec2 texCoordIn[];
out vec2 texCoord;

void main() {    
    gl_Position = gl_in[0].gl_Position;
	texCoord = texCoordIn[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
	texCoord = texCoordIn[1];
    EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	texCoord = texCoordIn[2];
    EmitVertex();
    
    EndPrimitive();

	gl_Position = gl_in[0].gl_Position;
	texCoord = texCoordIn[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
	texCoord = texCoordIn[1];
    EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	texCoord = texCoordIn[2];
    EmitVertex();
    
    EndPrimitive();
}  