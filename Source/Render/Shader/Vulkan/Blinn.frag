#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, row_major, binding = 2)
uniform cbSceneLight
{
   vec4 g_light_dir;
} sceneLight;

layout (std140, row_major, binding = 3)
uniform cbPerMaterial
{
   vec4 g_material_color;
   vec4 g_ambient_color;
   vec4 g_diffuse_color;
   vec4 g_specular_color;
   vec4 g_albedo_pow;
} perMaterial;


layout (location = 0) in vec3 normal;
layout (location = 1) in vec3 viewDir;

layout (location = 0) out vec4 oColor;

void main() 
{
   vec3 n = normalize(normal);
   vec3 v = normalize(viewDir);
   vec3 lightDir = -sceneLight.g_light_dir.xyz;
   vec3 halfDir = normalize(lightDir + v);

   vec3 ambient = perMaterial.g_ambient_color.rgb * perMaterial.g_albedo_pow.x;

   float diffuseLight = clamp(dot(lightDir, n), 0.0, 1.0);
   vec3 diffuse = perMaterial.g_diffuse_color.rgb * diffuseLight * perMaterial.g_albedo_pow.y;

   float specularLight = pow(clamp(dot(normal, halfDir), 0.0, 1.0), perMaterial.g_albedo_pow.w);
   vec3 specular = perMaterial.g_specular_color.rgb * specularLight * perMaterial.g_albedo_pow.z;

   vec4 materialColor = perMaterial.g_material_color;
//    float4 colorSample = float4(1.0, 1.0, 1.0, 1.0);//colorMap.sample(colorSampler, in.tex.xy);

   oColor = materialColor * vec4((ambient + diffuse + specular), 1.0);
}