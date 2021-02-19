#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 color;

layout (location = 0) out vec4 oColor;

void main() 
{
   float a = pow(color.a, 5);
   oColor = vec4(color.rgb, a);
}