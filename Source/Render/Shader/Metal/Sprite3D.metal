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
    float4 g_light_dir;
	float4 g_light_color;
} cbSceneLight;

typedef struct
{
    float4 g_material_color;
} cbPerMaterial;

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




vertex ColorInOut Sprite3DNormalVS(VertexNormal in [[stage_in]],
								   constant cbPerScene& perScene [[ buffer(0) ]],
								   constant cbPerModel& perModel [[ buffer(1) ]],
								   constant cbSceneLight& sceneLight [[ buffer(2) ]],
                                   constant cbPerMaterial& perMaterial [[ buffer(3) ]])
{
    ColorInOut out;

    float4 position = float4(in.pos, 1.0);
	out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
	
	float3 ligDir = float3(sceneLight.g_light_dir);
	float d = clamp(dot(-ligDir, in.normal), 0.0, 1.0);
	out.color = d * sceneLight.g_light_color + perMaterial.g_material_color;

    out.size = 3;
    
    return out;
}

vertex ColorInOut Sprite3DColorVS(VertexColor in [[stage_in]],
								  constant cbPerScene& perScene [[ buffer(0) ]],
								  constant cbPerModel& perModel [[ buffer(1) ]],
								  constant cbSceneLight& sceneLight [[ buffer(2) ]])
{
    ColorInOut out;

    float4 position = float4(in.pos, 1.0);
	out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
	
    out.color = in.color;

    out.size = 3;
    
    return out;
}

vertex TextureInOut Sprite3DTextureVS(VertexTexture in [[stage_in]],
									  constant cbPerScene& perScene [[ buffer(0) ]],
									  constant cbPerModel& perModel [[ buffer(1) ]],
									  constant cbSceneLight& sceneLight [[ buffer(2) ]])
{
	TextureInOut out;
    float4 position = float4(in.pos, 1.0);
	out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
	
	out.color = float4(1.0, 1.0, 1.0, 1.0);
	out.tex = in.tex;
	
	return out;
}

vertex TextureInOut Sprite3DTextureWithColorVS(VertexColorTexture in [[stage_in]],
											   constant cbPerScene& perScene [[ buffer(0) ]],
											   constant cbPerModel& perModel [[ buffer(1) ]],
											   constant cbSceneLight& sceneLight [[ buffer(2) ]])
{
	TextureInOut out;
    float4 position = float4(in.pos, 1.0);
	out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
	
    out.color = in.color;
	out.tex = in.tex;
	
	return out;
}

vertex TextureInOut Sprite3DTextureWithNormalVS(VertexNormalTexture in [[stage_in]],
												constant cbPerScene& perScene [[ buffer(0) ]],
												constant cbPerModel& perModel [[ buffer(1) ]],
												constant cbSceneLight& sceneLight [[ buffer(2) ]],
                                                constant cbPerMaterial& perMaterial [[ buffer(3) ]])
{
	TextureInOut out;
	
    float4 position = float4(in.pos, 1.0);
	out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
	
	float3 ligDir = float3(sceneLight.g_light_dir);
	float d = clamp(dot(-ligDir, in.normal), 0.0, 1.0);
	out.color = d * sceneLight.g_light_color + perMaterial.g_material_color;
	
	out.tex = in.tex;
	
	return out;
}

fragment float4 Sprite3DColorPS(ColorInOut in [[stage_in]])
{
    return in.color;
}

fragment float4 Sprite3DTexturePS(TextureInOut in [[stage_in]], texture2d<float> colorMap [[ texture(4) ]])
{
	constexpr sampler colorSampler(mip_filter::linear,
								   mag_filter::linear,
								   min_filter::linear);
	
	float4 colorSample = colorMap.sample(colorSampler, in.tex.xy);
	
	return colorSample;
}
