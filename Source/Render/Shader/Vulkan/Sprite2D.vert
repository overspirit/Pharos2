#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 7) in vec2 tex;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec2 oTex;

void main() 
{
    gl_Position = vec4(pos, 1.0);
    gl_Position.y = -gl_Position.y;

    oColor = color;
    oTex = tex;    
}