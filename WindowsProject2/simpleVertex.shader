#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec3 fv_color;


void main() {
	gl_Position = vec4(vertexPosition_modelspace * 0.75, 1.0);
	fv_color = vertexPosition_modelspace;
}
