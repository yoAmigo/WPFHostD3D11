#version 400

layout (vertices = 3) out;

in vec3 vtc_color[gl_MaxPatchVertices];
out vec3 tce_color[3];

void main()
{
	float Inner = 3.0;
	float Outer = 3.0;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tce_color[gl_InvocationID] = vtc_color[gl_InvocationID];

	gl_TessLevelOuter[0] = Outer;
	gl_TessLevelOuter[1] = Outer;
	gl_TessLevelOuter[2] = Outer;
	gl_TessLevelInner[0] = Inner;
}