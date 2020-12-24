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
    matrix_float4x4 g_boneMat[255];
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
    float3 normal [[attribute(2)]];
    float3 tangent [[attribute(4)]];
    float4 weights [[attribute(5)]];
    uint4 bones [[attribute(6)]];
    float2 tex [[attribute(7)]];
} Vertex;


typedef struct
{
	float4 pos [[position]];
	float3 normal;
    float3 tangent;
    float2 tex;
} VertexOut;


vertex VertexOut SkeletalVS(Vertex in [[stage_in]],
												constant cbPerScene& perScene [[ buffer(0) ]],
												constant cbPerModel& perModel [[ buffer(1) ]],
												constant cbSceneLight& sceneLight [[ buffer(2) ]])
{
    VertexOut out;
	
    float4 mypos = float4(0, 0, 0, 1.0);
    float4 mynormal = float4(0, 0, 0, 0);
    float4 mytangent = float4(0, 0, 0, 0);
    float4 inPos = float4(in.pos, 1.0);
    float4 inNormal = float4(in.normal, 0);
    float4 inTangent = float4(in.tangent, 0);
    
    uint iBone = in.bones.x;
    float fWeight = in.weights.x;
    matrix_float4x4 m = perModel.g_boneMat[iBone];
    mypos += (fWeight * m * inPos);
    mynormal += (fWeight * m * inNormal);
    mytangent += (fWeight * m * inTangent);

    iBone = in.bones.y;
    fWeight = in.weights.y;
    m = perModel.g_boneMat[iBone];
    mypos += (fWeight * m * inPos);
    mynormal += (fWeight * m * inNormal);
    mytangent += (fWeight * m * inTangent);
    
    iBone = in.bones.z;
    fWeight = in.weights.z;
    m = perModel.g_boneMat[iBone];
    mypos += (fWeight * m * inPos);
    mynormal += (fWeight * m * inNormal);
    mytangent += (fWeight * m * inTangent);
    
    iBone = in.bones.w;
    fWeight = in.weights.w;
    m = perModel.g_boneMat[iBone];
    mypos += (fWeight * m * inPos);
    mynormal += (fWeight * m * inNormal);
    mytangent += (fWeight * m * inTangent);
    
    out.pos = perScene.g_proj * perScene.g_view * perModel.g_world * mypos;
    
    out.normal = normalize(mynormal.xyz);
    out.tangent = normalize(mytangent.xyz);
    out.tex = in.tex;
	
	return out;
}


fragment float4 SkeletalPS(VertexOut in [[stage_in]], texture2d<float> colorMap [[ texture(4) ]])
{
    constexpr sampler colorSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    
    float4 colorSample = colorMap.sample(colorSampler, in.tex.xy);
    
    return colorSample;
}
