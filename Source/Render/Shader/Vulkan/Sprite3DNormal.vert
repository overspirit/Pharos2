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
} perModel;

layout (std140, row_major, binding = 2)
uniform cbSceneLight
{
    vec4 g_light_direction;
    vec4 g_light_color;
} sceneLight;

layout (std140, row_major, binding = 3)
uniform cbPerMaterial
{
    vec4 g_material_color;
} perMaterial;

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec4 oColor;

void main() 
{    
    gl_Position = vec4(pos, 1.0) * (perModel.g_world * perScene.g_view * perScene.g_proj);
    gl_Position.y = -gl_Position.y;

    vec3 lightDir = -sceneLight.g_light_direction.xyz;
    float d = clamp(dot(lightDir, normal), 0, 1.0);
    oColor = d * sceneLight.g_light_color + perMaterial.g_material_color;
}