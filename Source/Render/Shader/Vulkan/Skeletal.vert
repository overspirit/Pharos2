#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, row_major, binding = 0)
uniform cbPerScene
{
    mat4 g_view;
    mat4 g_proj;
} perScene;

layout (std140, row_major, binding = 1)
uniform cbPerModel
{
    mat4 g_world;
    mat4 g_boneMat[255];
} perModel;

layout (std140, row_major, binding = 2)
uniform cbSceneLight
{
    vec4 g_light_color;
    vec4 g_environment_color;
    vec3 g_light_direction;
} sceneLight;

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
    mat4 m = perModel.g_boneMat[iBone];
    mypos += (inPos * m * fWeight);
    mynormal += (normal * mat3(m) * fWeight);
    mytangent += (tangent * mat3(m) * fWeight);

    iBone = bones.y;
    fWeight = weights.y;
    m = perModel.g_boneMat[iBone];
    mypos += (inPos * m * fWeight);
    mynormal += (normal * mat3(m) * fWeight);
    mytangent += (tangent * mat3(m) * fWeight);

    iBone = bones.z;
    fWeight = weights.z;
    m = perModel.g_boneMat[iBone];
    mypos += (inPos * m * fWeight);
    mynormal += (normal * mat3(m) * fWeight);
    mytangent += (tangent * mat3(m) * fWeight);

    iBone = bones.w;
    fWeight = weights.w;
    m = perModel.g_boneMat[iBone];
    mypos += (inPos * m * fWeight);
    mynormal += (normal * mat3(m) * fWeight);
    mytangent += (tangent * mat3(m) * fWeight);
      
    gl_Position = mypos * (perModel.g_world * perScene.g_view * perScene.g_proj);
    gl_Position.y = -gl_Position.y;

    outNormal = normalize(mynormal);
    outTangent = normalize(mytangent);
    outTex = tex;
}