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
	matrix_float4x4 g_view;
	matrix_float4x4 g_proj;
} cbPerScene;


typedef struct
{
	matrix_float4x4 g_world;
} cbPerModel;


typedef struct
{
	float4 g_light_color;
	float4 g_environment_color;
	float4 g_light_direction;
} cbSceneLight;



typedef struct
{
    float3 pos [[attribute(0)]];
    float4 color [[attribute(1)]];
} VertexColor;

typedef struct
{
    float3 pos [[attribute(0)]];
    float3 normal [[attribute(2)]];
} VertexNormal;

typedef struct
{
    float3 pos [[attribute(0)]];
    float2 tex [[attribute(7)]];
} VertexTexture;

typedef struct
{
    float3 pos [[attribute(0)]];
    float4 color [[attribute(1)]];
	float2 tex [[attribute(7)]];
} VertexColorTexture;

typedef struct
{
    float3 pos [[attribute(0)]];
    float3 normal [[attribute(2)]];
	float2 tex [[attribute(7)]];
} VertexNormalTexture;

typedef struct
{
	float4 pos [[position]];
	float4 color;
    float size [[point_size]];
} ColorInOut;

typedef struct
{
	float4 pos [[position]];
	float4 color;
	float2 tex;
} TextureInOut;


vertex TextureInOut PhongVS(VertexNormalTexture in [[stage_in]],
												constant cbPerScene& perScene [[ buffer(0) ]],
												constant cbPerModel& perModel [[ buffer(1) ]],
												constant cbSceneLight& sceneLight [[ buffer(2) ]])
{
	TextureInOut out;
	
    float4 position = float4(in.pos, 1.0);
	out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
	
	float3 ligDir = float3(sceneLight.g_light_direction);
	float d = clamp(dot(-ligDir, in.normal), 0.0, 1.0);
	out.color = d * sceneLight.g_light_color + sceneLight.g_environment_color;
	
	out.tex = in.tex;
	
	return out;
}

fragment float4 PhongPS(TextureInOut in [[stage_in]], texture2d<float> colorMap [[ texture(4) ]])
{
	constexpr sampler colorSampler(mip_filter::linear,
								   mag_filter::linear,
								   min_filter::linear);
	
	float4 colorSample = colorMap.sample(colorSampler, in.tex.xy);
	
	return colorSample;
}
