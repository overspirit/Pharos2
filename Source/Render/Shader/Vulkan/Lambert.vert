#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, row_major, binding = 0)
uniform cbPerScene
{
    mat4 g_view;
    mat4 g_proj;
    vec4 g_camera_pos;
} perScene;

layout (std140, row_major, binding = 1)
uniform cbPerModel
{
    mat4 g_world;
} perModel;


layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

layout (location = 0) out vec4 oColor;
layout (location = 1) out vec3 oNormal;
layout (location = 2) out vec3 oViewDir;

void main() 
{
    vec4 worldPos = vec4(pos, 1.0) * perModel.g_world;

    gl_Position = worldPos * perScene.g_view * perScene.g_proj;
    gl_Position.y = -gl_Position.y;

    oColor = color;
    oNormal = (vec4(normal, 0.0) * perModel.g_world).xyz;
    oViewDir = (perScene.g_camera_pos - worldPos).xyz;
}