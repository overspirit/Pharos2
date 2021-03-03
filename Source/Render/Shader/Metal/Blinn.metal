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
    float4  g_camera_pos;
} cbPerScene;


typedef struct
{
	matrix_float4x4 g_world;
} cbPerModel;


typedef struct
{
    float4 g_light_dir;
} cbSceneLight;

typedef struct
{
    float4 g_material_color;
    float4 g_ambient_color;
    float4 g_diffuse_color;
    float4 g_specular_color;
    float4 g_albedo_pow;
} cbPerMaterial;

typedef struct
{
    float3 pos [[attribute(0)]];
    float3 normal [[attribute(2)]];
	//float2 tex [[attribute(7)]];
} Vertex;

typedef struct
{
	float4 pos [[position]];
	float3 normal;
    float3 viewDir;
	//float2 tex;
} TextureInOut;

vertex TextureInOut BlinnVS(Vertex in [[stage_in]],
                            constant cbPerScene& perScene [[ buffer(0) ]],
                            constant cbPerModel& perModel [[ buffer(1) ]])
{
	TextureInOut out;
	
    float4 worldPos = perModel.g_world * float4(in.pos, 1.0);
    
    worldPos /= worldPos.w;
    
	out.pos =  perScene.g_proj * perScene.g_view * worldPos;
    out.normal = (perModel.g_world * float4(in.normal, 0)).xyz;
    out.viewDir = (perScene.g_camera_pos - worldPos).xyz;
    //out.tex = in.tex;
	
	return out;
}

fragment float4 BlinnPS(TextureInOut in [[stage_in]],
                        texture2d<float> colorMap [[ texture(4) ]],
                        constant cbSceneLight& sceneLight [[ buffer(2) ]],
                        constant cbPerMaterial& perMaterial [[ buffer(3) ]])
{
//	constexpr sampler colorSampler(mip_filter::linear,
//								   mag_filter::linear,
//								   min_filter::linear);
	
    float3 normal = normalize(in.normal);
    float3 viewDir = normalize(in.viewDir);
    float3 lightDir = -sceneLight.g_light_dir.xyz;
    float3 halfDir = normalize(lightDir + viewDir);
        
    float3 ambient = perMaterial.g_albedo_pow.x * perMaterial.g_ambient_color.rgb;
    
    float diffuseLight = saturate(dot(normal, lightDir));
    float3 diffuse = perMaterial.g_albedo_pow.y * diffuseLight * perMaterial.g_diffuse_color.rgb;
    
    float specularLight = pow(saturate(dot(normal, halfDir)), perMaterial.g_albedo_pow.w);
    float3 specular = perMaterial.g_albedo_pow.z * specularLight * perMaterial.g_specular_color.rgb;
    
    float4 materialColor = perMaterial.g_material_color;
//    float4 colorSample = float4(1.0, 1.0, 1.0, 1.0);//colorMap.sample(colorSampler, in.tex.xy);
	
	return materialColor * float4((ambient + diffuse + specular), 1.0);
}
