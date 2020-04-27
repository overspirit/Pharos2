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

// Including header shared between this Metal shader code and Swift/C code executing Metal API commands
            
typedef struct
{
    matrix_float4x4 projectionMatrix;
    matrix_float4x4 modelViewMatrix;
} Uniforms;

using namespace metal;

typedef struct
{
    float3 position [[attribute(0)]];
    float4 color [[attribute(1)]];
    //float2 texCoord [[attribute(7)]];
} Vertex;

typedef struct
{
    float4 position [[position]];
    float4 color;
    //float2 texCoord;
} ColorInOut;

vertex ColorInOut vertexShader(Vertex in [[stage_in]], constant Uniforms& uniforms [[ buffer(2) ]])
{
    ColorInOut out;

    float4 position = float4(in.position, 1.0);
    out.position = uniforms.projectionMatrix * uniforms.modelViewMatrix * position;
    //out.texCoord = in.texCoord;
    out.color = in.color;

    return out;
}

fragment float4 fragmentShader(ColorInOut in [[stage_in]],
                               //constant Uniforms & uniforms [[ buffer(2) ]],
                               texture2d<half> colorMap     [[ texture(0) ]])
{
//    constexpr sampler colorSampler(mip_filter::linear,
//                                   mag_filter::linear,
//                                   min_filter::linear);
//
//    half4 colorSample = colorMap.sample(colorSampler, in.texCoord.xy);

    //return float4(colorSample);
    
    return in.color;
}
