#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 4) uniform sampler2D tex;

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec4 oColor;

void main() 
{
   vec4 texColor = textureLod(tex, texcoord, 0.0);
   oColor = vec4((texColor.rgb * color.rgb), texColor.a);
}