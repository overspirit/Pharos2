#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, row_major, binding = 0)
uniform bufferVals 
{
     mat4 view;
     mat4 proj;
} myBufferVals;

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec4 weights;
layout (location = 6) in uvec4 bones;
layout (location = 7) in vec2 tex;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec2 outTex;


void main() 
{    
    mat4 mvp = myBufferVals.view * myBufferVals.proj;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0) * mvp;//myBufferVals.mvp * pos;
    gl_Position.y = -gl_Position.y;
    outNormal = normal;
    outTex = vec2(tex.x, tex.y);
}