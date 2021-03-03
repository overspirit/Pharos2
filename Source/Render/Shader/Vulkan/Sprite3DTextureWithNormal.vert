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
    vec4 g_light_dir;
    vec4 g_light_color;
} sceneLight;

layout (std140, row_major, binding = 3)
uniform cbPerMaterial
{
    vec4 g_material_color;
} perMaterial;


layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;
layout (location = 7) in vec2 tex;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec2 oTex;


void main() 
{    
    gl_Position = vec4(pos, 1.0) * (perModel.g_world * perScene.g_view * perScene.g_proj);
    gl_Position.y = -gl_Position.y;

    vec3 lightDir = -sceneLight.g_light_dir.xyz;
    float d = dot(lightDir, normal);
    oColor = d * sceneLight.g_light_color + perMaterial.g_material_color;
    oTex = tex;
}