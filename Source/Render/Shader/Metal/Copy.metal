//
//  Shaders.metal
//  ModelViewer
//
//  Created by xiaowenzhang on 2020/4/7.
//  Copyright © 2020 mindbrain. All rights reserved.
//

// File for Metal kernel and shader functions

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

typedef struct
{
    float3 pos [[attribute(0)]];
    float2 tex [[attribute(7)]];
} VertexTexture;

typedef struct
{
	float4 pos [[position]];
	float2 tex;
} TextureInOut;


vertex TextureInOut CopyVS(VertexTexture in [[stage_in]])
{
	TextureInOut out;
    out.pos = float4(in.pos, 1.0);
	out.tex = in.tex;
	
	return out;
}

fragment float4 CopyPS(TextureInOut in [[stage_in]], texture2d<float> colorMap [[ texture(0) ]])
{
    constexpr sampler colorSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    
    float4 colorSample = colorMap.sample(colorSampler, in.tex.xy);
    
    return colorSample;
}
