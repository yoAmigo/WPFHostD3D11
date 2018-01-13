#version 400

layout(triangles, equal_spacing, ccw) in;

out vec3 tePatchDistance;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	float w = gl_TessCoord.z;

	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;

	tePatchDistance = gl_TessCoord;
	gl_Position = vec4(u * p0 + v * p1 + w * p2, 1.0);
}