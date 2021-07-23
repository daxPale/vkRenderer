#version 450	
#extension GL_ARB_separate_shader_objects : enable
#pragma shader_stage(vertex)

layout(location = 0) in vec2 position;

void main()
{
	gl_Position = vec4(position, 0.0f, 1.0f);
}