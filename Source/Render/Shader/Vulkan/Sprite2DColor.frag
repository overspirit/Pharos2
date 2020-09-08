#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D tex;

layout (std140, row_major, binding = 1)
uniform cbPerDraw
{
   float round;
   float scale;
} perDraw;

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texcoord;

layout (location = 0) out vec4 oColor;

bool isOut(float x, float y, float cx, float cy, float r)
{
   bool flag1 = (cx<0.5f && x<cx) || (cx>0.5f && x>cx);
   bool flag2 = (cy<0.5f && y<cy) || (cy>0.5f && y>cy);
   float dis = (x-cx)*(x-cx) + (y-cy)*(y-cy);
   if(flag1 && flag2 && dis>r*r)
   {
      return true;
   }

   return false;
}

void main() 
{
   float r = perDraw.round;
   float s = perDraw.scale;

   float x = texcoord.x;
   float y = texcoord.y;

   float rate = 1.0;

   if (r > 0)
   {
      if (isOut(x * s, y, r, r, r)) rate = 0.0;
      if (isOut(x * s, y, r, 1-r, r)) rate = 0.0;
      if (isOut(x * s, y, (1*s-r), r, r)) rate = 0.0;
      if (isOut(x * s, y, (1*s-r), 1-r, r)) rate = 0.0;
   }

   oColor = vec4(color.rgb, rate * color.a);
}