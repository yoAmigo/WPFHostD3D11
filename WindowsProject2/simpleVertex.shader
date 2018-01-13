#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec3 vtc_color;


void main() {
	gl_Position = vec4(vertexPosition_modelspace * 0.75, 1.0);
	vtc_color = vertexPosition_modelspace;
}
