#version 400

layout(triangles, invocations = 1) in;
//layout(triangle_strip, max_vertices = 9) out;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tcg_color[3];

out vec3 gf_color;

void main() {
	for (int i = 0; i < 3; i++)
	{
		if (gl_InvocationID == 0)
		{
			gf_color = tcg_color[i];
			gl_Position = gl_in[i].gl_Position + vec4(0.0, -0.1, 0.0, 0.0);
			EmitVertex();
		}
		else if (gl_InvocationID == 1)
		{
			gf_color = tcg_color[gl_InvocationID];
			gl_Position = gl_in[i].gl_Position + vec4(-0.1, 0.1, 0.0, 0.0);
			EmitVertex();
		}
		else
		{
			gf_color = tcg_color[gl_InvocationID];
			gl_Position = gl_in[i].gl_Position + vec4(0.1, 0.1, 0.0, 0.0);
			EmitVertex();
		}
		//EndPrimitive();
	}
	EndPrimitive();
}
