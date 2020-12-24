//
//  Shaders.metal
//  ModelViewer
//
//  Created by v_xiaowenzhang on 2020/4/7.
//  Copyright © 2020 mindbrain. All rights reserved.
//

// File for Metal kernel and shader functions

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

typedef struct
{
    float2 font_distance;
    float2 font_offset;
    float4 char_color;
    float4 outline_color;
    float outline_scale;
} cbFontInfo;


typedef struct
{
    float3 pos [[attribute(0)]];
    float4 color [[attribute(1)]];
	float2 tex [[attribute(7)]];
} VertexColorTexture;


typedef struct
{
	float4 pos [[position]];
	float4 color;
	float2 tex;
} TextureInOut;


vertex TextureInOut FontVS(VertexColorTexture in [[stage_in]], constant cbFontInfo& perFont [[ buffer(1) ]])
{
	TextureInOut out;
	
    out.pos = float4(in.pos, 1.0);
    out.pos.x += perFont.font_offset.x;
    out.pos.y += perFont.font_offset.y;
    
    out.color = in.color;
	out.tex = in.tex;
	
	return out;
}

fragment float4 FontPS(TextureInOut in [[stage_in]], constant cbFontInfo& perFont [[ buffer(2) ]], texture2d<float> colorMap [[ texture(0) ]])
{
    constexpr sampler colorSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
        
    float alpha = colorMap.sample(colorSampler, in.tex).a;
        
    float fontBase = perFont.font_distance.x;
    float fontScale = perFont.font_distance.y;
    float outline_scale = fontScale + perFont.outline_scale;
    
    float4 finalColor;
    
    if(alpha < (-fontBase / fontScale) && alpha > (-fontBase / outline_scale))
    {
       finalColor = perFont.outline_color;
    }
    else
    {
       finalColor = perFont.char_color;
    }
       
    return float4(finalColor.xyz, alpha * outline_scale + fontBase) * in.color;
}
