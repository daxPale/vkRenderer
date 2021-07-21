#version 450	
#extension GL_ARB_separate_shader_objects : enable
#pragma shader_stage(vertex)

vec2 positions[3] = vec2[]
(
	vec2(0.0f, -0.5f),
	vec2(0.5f, 0.5f),
	vec2(0.5f, -0.5f)
);

void main()
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
}