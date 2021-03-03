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
    float3 pos [[attribute(0)]];
    float4 color [[attribute(1)]];
} VertexColor;

typedef struct
{
    float4 pos [[position]];
    float4 color;
    float size [[point_size]];
} ColorInOut;


vertex ColorInOut BorderShineVS(VertexColor in [[stage_in]],
                                  constant cbPerScene& perScene [[ buffer(0) ]],
                                  constant cbPerModel& perModel [[ buffer(1) ]])
{
    ColorInOut out;

    float4 position = float4(in.pos, 1.0);
    out.pos =  perScene.g_proj * perScene.g_view * perModel.g_world * position;
    
    out.color = in.color;

    out.size = 3;
    
    return out;
}

fragment float4 BorderShinePS(ColorInOut in [[stage_in]])
{
    float a = pow(in.color.a, 5);
    return float4(in.color.rgb, a);
}
