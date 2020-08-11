#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, row_major, binding = 2)
uniform cbFontInfo
{
    vec2 font_distance;
    vec2 font_offset;
    vec4 char_color;
    vec4 outline_color;
    float outline_scale;
} perFont;

layout (binding = 0) uniform sampler2D tex;

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec4 oColor;

void main() 
{
   float alpha = textureLod(tex, texcoord, 0.0).x;

   // oColor = vec4(alpha, alpha, alpha, 1.0);

   float fontBase = perFont.font_distance.x;
   float fontScale = perFont.font_distance.y;
   float outline_scale = fontScale + perFont.outline_scale;
   
   vec4 finalColor;
   
   if(alpha < (-fontBase / fontScale) && alpha > (-fontBase / outline_scale))
   {
      finalColor = perFont.outline_color;
   }
   else
   {
      finalColor = perFont.char_color;
   }
      
   oColor = vec4(finalColor.xyz, alpha * outline_scale + fontBase) * color; 

}