layout(vertices = 3) out;

void main()
{
	float Inner = 3.0;
	float Outer = 3.0;

	gl_out[gl_invocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	gl_TessLevelOuter[0] = Outer;
	gl_TessLevelOuter[1] = Outer;
	gl_TessLevelOuter[2] = Outer;
	gl_TessLevelInner[0] = Inner;
}