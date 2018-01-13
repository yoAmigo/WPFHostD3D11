#version 330 core

layout(triangles) in;
//layout(triangle_strip, max_vertices = 9) out;
layout(triangle_strip, max_vertices = 3) out;

in vec3    fv_color[3];

out vec3 f_color;

void main() {
	for (int i = 0; i < 3; i++)
	{
		//f_color = fv_color[2];
		//gl_Position = gl_in[i].gl_Position + vec4(0.0, -0.1, 0.0, 0.0);
		//EmitVertex();
		f_color = fv_color[1];
		gl_Position = gl_in[i].gl_Position + vec4(-0.1, 0.1, 0.0, 0.0);
		EmitVertex();
		//f_color = fv_color[0];
		//gl_Position = gl_in[i].gl_Position + vec4(0.1 , 0.1, 0.0, 0.0);
		//EmitVertex();
		//EndPrimitive();
	}
	EndPrimitive();
}
