#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec3 f_color;

void main() {
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;
	f_color = vertexPosition_modelspace;
}
