// This file is part of the FidelityFX SDK.
//
// Copyright (C) 2025 Advanced Micro Devices, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "common.hlsl"
#include "raytracing_common.hlsl"

Texture2D<float4> g_DirectDiffuse : register(t0);
Texture2D<float4> g_DirectSpecular : register(t1);
Texture2D<float4> g_IndirectDiffuse : register(t2);
Texture2D<float4> g_IndirectSpecular : register(t3);
Texture2D<float4> g_DominantLightVisibility : register(t4);
Texture2D<float4> g_SkipSignal : register(t5);

Texture2D<float4> g_DiffuseAlbedo : register(t6);
Texture2D<float4> g_SpecularAlbedo : register(t7);
Texture2D<float4> g_FusedAlbedo : register(t8);
Texture2D<float4> g_Normals : register(t9);
Texture2D<float> g_Depthbuffer : register(t10);

RWTexture2D<float4> g_Output : register(u0);

cbuffer Constants : register(b0)
{
    float4x4 g_ClipToWorld;
    float4x4 g_CameraToWorld;
    
    float g_DirectDiffuseContrib;
    float g_DirectSpecularContrib;
    float g_IndirectDiffuseContrib;
    float g_IndirectSpecularContrib;
    
    float g_SkipContrib;
    float g_RangeMin;
    float g_RangeMax;
    uint g_Flags;
    
    float4 g_ChannelContrib;

    float2 g_InvRenderSize;
    uint g_UseDominantLight;
    uint g_DominantLightIndex;
};

ConstantBuffer<SceneLightingInformation> g_LightInfo : register(b1);

#define COMPOSE_DEBUG_MODE 0x1
#define COMPOSE_DEBUG_USE_RANGE 0x2
#define COMPOSE_DEBUG_DECODE_SQRT 0x4
#define COMPOSE_DEBUG_ABS_VALUE 0x8
#define COMPOSE_DEBUG_DECODE_NORMALS 0x10
#define COMPOSE_DEBUG_ONLY_FIRST_RESOURCE 0x20
#define COMPOSE_FUSED 0x40

[numthreads(8, 8, 1)]
void main(uint3 dtid : SV_DispatchThreadID)
{
    uint2 pixel = dtid.xy;

    float z = g_Depthbuffer.Load(int3(pixel.xy, 0));
    float3 screenUVW = float3((float2(pixel) + float2(0.5f, 0.5f)) * g_InvRenderSize, z);
    float3 pixelPosition = ScreenSpaceToWorldSpace(float3(screenUVW.xy, 1.0f), g_ClipToWorld);
    const float3 cameraPosition = ViewSpaceToWorldSpace(float4(0.0f, 0.0f, 0.0f, 1.0f), g_CameraToWorld);
    const float3 toCameraDirection = normalize(cameraPosition - pixelPosition);
    
    float4 directDiffuse = g_DirectDiffuse[pixel] * g_DirectDiffuseContrib;
    float3 directSpecular = g_DirectSpecular[pixel].xyz * g_DirectSpecularContrib;
    float3 indirectDiffuse = g_IndirectDiffuse[pixel].xyz * g_IndirectDiffuseContrib;
    float3 indirectSpecular = g_IndirectSpecular[pixel].xyz * g_IndirectSpecularContrib;
    float3 skip = g_SkipSignal[pixel].xyz * g_SkipContrib;
    
    const bool debugOnlyFirstResource = (g_Flags & COMPOSE_DEBUG_ONLY_FIRST_RESOURCE) != 0;
    
    float4 diffuseAlbedo = Square(g_DiffuseAlbedo[pixel]);
    float4 specularAlbedo = Square(g_SpecularAlbedo[pixel]);
    
    if (g_UseDominantLight && !debugOnlyFirstResource)
    {
        float dominantLightVisibility = saturate(g_DominantLightVisibility[pixel].x);
        if (dominantLightVisibility > 0.0f)
        {
            const float3 normalEnc = g_Normals[pixel].xyz;
            float3 normal = OctahedronUvToNormal(normalEnc.xy);
            float roughness = normalEnc.z;
            
            MaterialInfo materialInfo;
            materialInfo.perceptualRoughness = max(roughness, 1e-6);
            materialInfo.alphaRoughness = max(roughness * roughness, 1e-6);
            materialInfo.metallic = diffuseAlbedo.a;
            materialInfo.baseColor = diffuseAlbedo.rgb * (1.0 - materialInfo.metallic);
            materialInfo.reflectance0 = lerp((float3)MinReflectance, diffuseAlbedo.rgb, (float3)materialInfo.metallic);
            materialInfo.reflectance90 = float3(1.0, 1.0, 1.0);
            
            LightingResult lightResult = DirectionalLight(g_LightInfo.LightInfo[g_DominantLightIndex], materialInfo, normal, toCameraDirection);
            directDiffuse.xyz += lightResult.diffuse * dominantLightVisibility;
            directSpecular.xyz += lightResult.specular * dominantLightVisibility;
        }
    }
    
    if (g_Flags & COMPOSE_DEBUG_MODE)
    {
        if (debugOnlyFirstResource)
        {
            if (g_Flags & COMPOSE_DEBUG_DECODE_SQRT)
            {
                directDiffuse = Square(directDiffuse);
            }
            if (g_Flags & COMPOSE_DEBUG_ABS_VALUE)
            {
                directDiffuse = abs(directDiffuse);
            }
            if (g_Flags & COMPOSE_DEBUG_USE_RANGE)
            {
                directDiffuse = smoothstep(g_RangeMin, g_RangeMax, directDiffuse);
            }
            if (g_Flags & COMPOSE_DEBUG_DECODE_NORMALS)
            {
                directDiffuse.w = directDiffuse.z;
                directDiffuse.xyz = OctahedronUvToNormal(directDiffuse.xy) * 0.5f + 0.5f;
            }
            if (all(g_ChannelContrib.xyz == 0.0f) && (g_ChannelContrib.a == 1.0f))
            {
                directDiffuse = directDiffuse.aaaa;
            }
            else
            {
                directDiffuse *= g_ChannelContrib;
            }
            g_Output[pixel] = directDiffuse;
        }
        else
        {
            g_Output[pixel] = float4(skip + directDiffuse.xyz + indirectDiffuse + directSpecular + indirectSpecular, 0.0f);
        }
    }
    else
    {
        float3 result = float3(0,0,0);
        if (g_Flags & COMPOSE_FUSED)
        {
            float4 fusedAlbedo = Square(g_FusedAlbedo[pixel]);
            result = (directSpecular.xyz * specularAlbedo.xyz) + (directDiffuse.xyz * diffuseAlbedo.xyz * (1.0 - diffuseAlbedo.w)) + indirectSpecular.xyz * fusedAlbedo.xyz;
        }
        else
        {
            float3 diffuse = (directDiffuse.xyz + indirectDiffuse) * diffuseAlbedo.xyz * (1.0f - diffuseAlbedo.w);
            float3 specular = (directSpecular + indirectSpecular) * specularAlbedo.xyz;
            result = diffuse + specular;
        }
        
        g_Output[pixel] = float4(skip + result, 0.0f);
    }
}
