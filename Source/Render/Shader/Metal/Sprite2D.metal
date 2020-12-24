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


vertex TextureInOut Sprite2DVS(VertexColorTexture in [[stage_in]])
{
	TextureInOut out;
    out.pos = float4(in.pos, 1.0);
	out.color = in.color;
	out.tex = in.tex;
	
	return out;
}

fragment float4 Sprite2DColorPS(TextureInOut in [[stage_in]])
{
	return in.color;
}

fragment float4 Sprite2DTexturePS(TextureInOut in [[stage_in]], texture2d<float> colorMap [[ texture(0) ]])
{
	constexpr sampler colorSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
	
	float4 colorSample = colorMap.sample(colorSampler, in.tex.xy);
	
	return colorSample;
}
