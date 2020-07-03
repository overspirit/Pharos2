#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform sampler2D tex;

layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec4 outColor;

void main() 
{
   vec3 lightDir1 = normalize(vec3(1.0, 1.0, 1.0));
   vec3 lightDir2 = normalize(vec3(1.0, 1.0, -1.0));
   vec3 lightDir3 = normalize(vec3(-1.0, 1.0, 1.0));
   vec3 lightDir4 = normalize(vec3(-1.0, 1.0, -1.0));
   vec3 lightDir5 = normalize(vec3(1.0, -1.0, 1.0));
   vec3 lightDir6 = normalize(vec3(1.0, -1.0, -1.0));
   vec3 lightDir7 = normalize(vec3(-1.0, -1.0, 1.0));
   vec3 lightDir8 = normalize(vec3(-1.0, -1.0, -1.0));

   vec3 n = normalize(normal);
   float d1 = clamp(dot(n, lightDir1) + dot(n, lightDir2) + dot(n, lightDir3) + dot(n, lightDir4), 0, 1.0);
   float d2 = clamp(dot(n, lightDir5) + dot(n, lightDir6) + dot(n, lightDir7) + dot(n, lightDir8), 0, 1.0);
   float d = clamp(dot(n, lightDir1), 0, 1.0) + 0.31;
   outColor = vec4(d, d, d, 1.0);
   //outColor = textureLod(tex, texcoord, 0.0);// * color;
}