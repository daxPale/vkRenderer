#version 450	
#extension GL_ARB_separate_shader_objects : enable
#pragma shader_stage(vertex)

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;
void main()
{
	fragColor = color;
	gl_Position = vec4(position, 0.0f, 1.0f);
}