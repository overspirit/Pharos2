#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, row_major, binding = 0)
uniform bufferVals 
{
    mat4 view;
    mat4 proj;
} myBufferVals;

layout (std140, row_major, binding = 1)
uniform boneVals 
{
    mat4 boneMat[255];
} myBoneVals;

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec4 weights;
layout (location = 6) in uvec4 bones;
layout (location = 7) in vec2 tex;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outTangent;
layout (location = 2) out vec2 outTex;

void main() 
{
    vec4 mypos = vec4(0, 0, 0, 1.0);
    vec3 mynormal = vec3(0, 0, 0);
    vec3 mytangent = vec3(0, 0, 0);
    vec4 inPos = vec4(pos.x, pos.y, pos.z, 1.0);

    uint iBone = bones.x;
    float fWeight = weights.x;
    mat4 m = myBoneVals.boneMat[iBone];
    mypos += fWeight * (inPos * m);
    mynormal += fWeight * (normal * mat3(m));
    mytangent += fWeight * (tangent * mat3(m));

    iBone = bones.y;
    fWeight = weights.y;
    m = myBoneVals.boneMat[iBone];
    mypos += fWeight * (inPos * m);
    mynormal += fWeight * (normal * mat3(m));
    mytangent += fWeight * (tangent * mat3(m));

    iBone = bones.z;
    fWeight = weights.z;
    m = myBoneVals.boneMat[iBone];
    mypos += fWeight * (inPos * m);
    mynormal += fWeight * (normal * mat3(m));
    mytangent += fWeight * (tangent * mat3(m));

    iBone = bones.w;
    fWeight = weights.w;
    m = myBoneVals.boneMat[iBone];
    mypos += fWeight * (inPos * m);
    mynormal += fWeight * (normal * mat3(m));
    mytangent += fWeight * (tangent * mat3(m));
      
    mat4 mvp = myBufferVals.view * myBufferVals.proj;
    gl_Position = mypos * mvp;//myBufferVals.mvp * pos;
    gl_Position.y = -gl_Position.y;

    outNormal = normalize(mynormal);
    outTangent = normalize(mytangent);
    outTex = tex;
}