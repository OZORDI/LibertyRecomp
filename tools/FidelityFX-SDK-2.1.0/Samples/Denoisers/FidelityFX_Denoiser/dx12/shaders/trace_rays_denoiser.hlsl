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

ConstantBuffer<TraceRaysConstants> Constants : register(b0);
ConstantBuffer<SceneInformation> SceneInfo : register(b1);
ConstantBuffer<SceneLightingInformation> LightInfo : register(b2);

RaytracingAccelerationStructure g_TLAS : register(t0);
Texture2D<float> g_Depthbuffer : register(t1);

Texture2D<float4> g_BrdfTexture : register(t2);
TextureCube g_PrefilteredCube : register(t3);
TextureCube g_IrradianceCube : register(t4);

#define DECLARE_SRV_REGISTER(idx)     t##idx
#define DECLARE_UAV_REGISTER(idx)     u##idx
#define DECLARE_SAMPLER_REGISTER(idx) s##idx

#define DECLARE_SRV(slot) register(DECLARE_SRV_REGISTER(slot))
#define DECLARE_UAV(slot) register(DECLARE_UAV_REGISTER(slot))
#define DECLARE_SAMPLER(slot) register(DECLARE_SAMPLER_REGISTER(slot))

StructuredBuffer<PTMaterialInfo> g_MaterialInfo : DECLARE_SRV(RAYTRACING_INFO_MATERIAL);
StructuredBuffer<PTInstanceInfo> g_InstanceInfo : DECLARE_SRV(RAYTRACING_INFO_INSTANCE);
StructuredBuffer<uint> g_SurfaceId : DECLARE_SRV(RAYTRACING_INFO_SURFACE_ID);
StructuredBuffer<PTSurfaceInfo> g_SurfaceInfo : DECLARE_SRV(RAYTRACING_INFO_SURFACE);
StructuredBuffer<uint> g_IndexBuffers[MAX_BUFFER_COUNT] : DECLARE_SRV(INDEX_BUFFER_BEGIN_SLOT);
StructuredBuffer<float> g_VertexBuffers[MAX_BUFFER_COUNT] : DECLARE_SRV(VERTEX_BUFFER_BEGIN_SLOT);

Texture2D g_ShadowMapTextures[MAX_SHADOW_MAP_TEXTURES_COUNT] : DECLARE_SRV(SHADOW_MAP_BEGIN_SLOT);
Texture2D g_Textures[MAX_TEXTURES_COUNT] : DECLARE_SRV(TEXTURE_BEGIN_SLOT);

SamplerState g_SamplerBRDF : register(s0);
SamplerState g_SamplerIrradianceCube : register(s1);
SamplerState g_SamplerPrefilteredCube : register(s2);
SamplerComparisonState SamShadow : register(s3); // needs to be named this to function in PBRLighting in lightingcommon.hlsl
SamplerState g_Samplers[MAX_SAMPLERS_COUNT] : DECLARE_SAMPLER(SAMPLER_BEGIN_SLOT);

RWTexture2D<float4> g_DirectSpecularTarget : register(u0);
RWTexture2D<float4> g_DirectDiffuseTarget : register(u1);
RWTexture2D<float4> g_IndirectSpecularTarget : register(u2);
RWTexture2D<float4> g_IndirectDiffuseTarget : register(u3);
RWTexture2D<float> g_DominantLightVisibilityTarget : register(u4);
RWTexture2D<float4> g_DiffuseAlbedoTarget : register(u5);
RWTexture2D<float4> g_SpecularAlbedoTarget : register(u6);
RWTexture2D<float4> g_FusedAlbedoTarget : register(u7);
RWTexture2D<float4> g_NormalsTarget : register(u8);
RWTexture2D<float4> g_SkipTarget : register(u9);

#define brdfTexture g_BrdfTexture
float4 SampleBRDFTexture(float2 uv)
{
    return g_BrdfTexture.SampleLevel(g_SamplerBRDF, uv, 0);
}

#define irradianceCube g_IrradianceCube
float4 SampleIrradianceCube(float3 n)
{
    return g_IrradianceCube.SampleLevel(g_SamplerIrradianceCube, n, 0);
}

#define prefilteredCube g_PrefilteredCube
float4 SamplePrefilteredCube(float3 reflection, float lod)
{
    return g_PrefilteredCube.SampleLevel(g_SamplerPrefilteredCube, reflection, lod);
}

#define IBL_INDEX b3
#include "lightingcommon.h"

#include "raytracing_common.hlsl"

LightingResult CalculateLighting(inout uint rngState, out float dominantLightVisibility, float3 worldPosition, float3 normal, float3 view, MaterialInfo materialInfo, uint recursionIndex)
{
    // Accumulate contribution from punctual lights
    LightingResult totalResult = (LightingResult)0;
    for (int i = 0; i < LightInfo.LightCount; ++i)
    {
        LightInformation lightInfo = LightInfo.LightInfo[i];
        float shadowFactor = GetShadowFactor(g_TLAS, lightInfo, rngState, worldPosition, normal, recursionIndex == 0);
        if (Constants.use_dominant_light && recursionIndex == 0 && i == Constants.dominant_light_index)
        {
            dominantLightVisibility = shadowFactor;
            continue;
        }
        
        if (shadowFactor > 0.0f)
        {
            LightingResult lightResult;
            if (recursionIndex > 0)
            {
                // Apply fully albedo modulated shading
                lightResult = ApplyPunctualLight(worldPosition, normal, view, materialInfo, lightInfo);
            }
            else
            {
                // Apply only lighting contribution
                lightResult = ApplyLight(worldPosition, normal, view, materialInfo, lightInfo);
            }
            totalResult.diffuse += lightResult.diffuse * shadowFactor;
            totalResult.specular += lightResult.specular * shadowFactor;
        }
    }
    
    return totalResult;
}

void TraceRay_OnHit(inout TraceRayHitResult result, in TraceRayDesc ray, in TraceRayHitInfo hitInfo, inout uint rngState)
{
    PTInstanceInfo instanceInfo = g_InstanceInfo.Load(hitInfo.instanceId);
    
    uint surfaceId = g_SurfaceId.Load((instanceInfo.surface_id_table_offset + hitInfo.geometryIndex));
    PTSurfaceInfo surfaceInfo = g_SurfaceInfo.Load(surfaceId);
    
    uint3 faceIndices;
    if (surfaceInfo.index_type == SURFACE_INFO_INDEX_TYPE_U16)
    {
        faceIndices = FetchFaceIndicesU16(g_IndexBuffers, surfaceInfo.index_offset, hitInfo.triangleId);
    }
    else // SURFACE_INFO_INDEX_TYPE_U32
    {
        faceIndices = FetchFaceIndicesU32(g_IndexBuffers, surfaceInfo.index_offset, hitInfo.triangleId);
    }
    
    LocalBasis localBasis = FetchLocalBasis(g_VertexBuffers, surfaceInfo, hitInfo.objectToWorld, faceIndices, hitInfo.barycentrics, hitInfo.isFrontFace);
    
    PTMaterialInfo material = g_MaterialInfo.Load(surfaceInfo.material_id);
    
    float mipLevel = CalculateMipLevelFromRayHit(hitInfo.rayT, localBasis);
    PBRPixelInfo pbrPixelInfo = GetPBRPixelInfoFromMaterial(g_Textures, g_Samplers, localBasis, material, mipLevel);
    pbrPixelInfo.pixelWorldPos = float4(hitInfo.position, 1.0f);
    pbrPixelInfo.pixelCoordinates = float4(0, 0, 0, 0);
        
    MaterialInfo materialInfo = GetMaterialInfo(pbrPixelInfo);
    float dominantLightVisibility = 0.0f;
    LightingResult lightingResult = CalculateLighting(rngState, dominantLightVisibility, pbrPixelInfo.pixelWorldPos.xyz, pbrPixelInfo.pixelNormal.xyz, -ray.direction, materialInfo, ray.recursionIndex);
        
    float3 emission = float3(0, 0, 0);
    //if (Constants.use_emission)
    {
        emission = float3(material.emission_factor_x, material.emission_factor_y, material.emission_factor_z) /* * Constants.emissive_factor*/;
        if (material.emission_tex_id >= 0)
        {
            emission = emission * g_Textures[NonUniformResourceIndex(material.emission_tex_id)].SampleLevel(g_Samplers[material.emission_tex_sampler_id], localBasis.uv, 0).xyz;
        }
    }

    result.radiance = lightingResult.diffuse + lightingResult.specular + emission;
    result.diffuseRadiance = lightingResult.diffuse;
    result.specularRadiance = lightingResult.specular;
    result.emission = emission;
    result.materialInfo = materialInfo;
    result.localBasis = localBasis;
    result.worldPosition = pbrPixelInfo.pixelWorldPos.xyz;
    result.worldNormal = pbrPixelInfo.pixelNormal.xyz;
    result.dominantLightVisibility = dominantLightVisibility;
}

void TraceRay_OnMiss(inout TraceRayHitResult result, in TraceRayDesc ray, inout uint rngState)
{
    // Sample radiance cache here...
    result.radiance = SamplePrefilteredCube(ray.direction, 0.0f).xyz * Constants.ibl_factor;
}

[numthreads(8, 8, 1)]
void main(uint3 dtid : SV_DispatchThreadID)
{
    const uint2 pixel = dtid.xy;

    float z = g_Depthbuffer.Load(int3(pixel.xy, 0));
    float3 screenUVW = float3((float2(pixel) + float2(0.5f, 0.5f)) * Constants.inv_render_size, z);
    float3 pixelPosition = ScreenSpaceToWorldSpace(float3(screenUVW.xy, 1.0f), Constants.clip_to_world);
    const float3 cameraPosition = ViewSpaceToWorldSpace(float4(0.0f, 0.0f, 0.0f, 1.0f), Constants.camera_to_world);
    const float3 toCameraDirection = normalize(cameraPosition - pixelPosition);

    uint rngState = GenerateRngState(dtid.y, dtid.x, Constants.frame_index + 1);
    
    // Trace primary ray
    TraceRayDesc ray;
    ray.origin = cameraPosition;
    ray.direction = -toCameraDirection;
    ray.tMin = 0.01f;
    ray.tMax = 1024.0f;
    ray.recursionIndex = 0;

    TraceRayHitResult primaryHit = TraceRay(g_TLAS, ray, rngState);
    if (primaryHit.hit)
    {
        MaterialInfo material = primaryHit.materialInfo;
        float roughness = min(0.99f, saturate(material.perceptualRoughness));
        
        const float3x3 TBN = CreateTBN(primaryHit.worldNormal);
                
        float3 indirectSpecularSample = float3(0, 0, 0);
        float3 indirectSpecularRayDir = float3(0, 0, 0);
        if (1)
        {
            // Trace indirect specular ray
            float3 rd = SampleReflectionVectorReroll(rngState, -ray.direction, primaryHit.worldNormal, roughness);
            indirectSpecularRayDir = rd;
            
            TraceRayDesc secondaryRay = ray;
            secondaryRay.origin = primaryHit.worldPosition + primaryHit.worldNormal * EPSILON;
            secondaryRay.direction = rd;
            secondaryRay.recursionIndex = 1;
            
            TraceRayHitResult specularHit = TraceRay(g_TLAS, secondaryRay, rngState);
            indirectSpecularSample.xyz = specularHit.radiance;
        }
        
        float3 indirectDiffuseSample = float3(0, 0, 0);
        float3 indirectDiffuseRayDir = float3(0, 0, 0);
        if (1)
        {
            // Trace indirect diffuse ray
            float3 rd = normalize(mul(CosineSampleHemisphere(rngState), TBN));
            indirectDiffuseRayDir = rd;
            
            TraceRayDesc secondaryRay = ray;
            secondaryRay.origin = primaryHit.worldPosition + primaryHit.worldNormal * EPSILON;
            secondaryRay.direction = rd;
            secondaryRay.recursionIndex = 1;
            
            TraceRayHitResult diffuseHit = TraceRay(g_TLAS, secondaryRay, rngState);
            indirectDiffuseSample.xyz = diffuseHit.radiance;
        }

        float roll = RandomFloat01(rngState);
        
        // From getIBLContribution() in lightingcommon.h
        // In Vulkan and DirectX (0, 0) is on the top left corner. We need to flip the y-axis as the brdf lut has roughness y-up.
        float NoV = dot(primaryHit.worldNormal, toCameraDirection);
        float2 brdfSamplePoint = clamp(float2(NoV, 1.0 - material.perceptualRoughness), float2(0.0, 0.0), float2(1.0, 1.0));
        // retrieve a scale and bias to F0. See [1], Figure 3
        float2 brdf = SampleBRDFTexture(brdfSamplePoint).rg;
        
        float3 specularAlbedo = material.reflectance0 * brdf.x + brdf.y;
        float4 specularAlbedo_NoV = abs(float4(specularAlbedo, NoV) + lerp(-1.0f, 1.0f, roll).xxxx * 1.0f / 256.0f);
        float4 diffuseAlbedo_Metallic = float4(material.baseColor, material.metallic);
        float3 fusedModulator = max(1e-3, max(specularAlbedo_NoV.xyz, diffuseAlbedo_Metallic.xyz));
        g_SpecularAlbedoTarget[pixel] = sqrt(specularAlbedo_NoV);
        g_DiffuseAlbedoTarget[pixel] = sqrt(diffuseAlbedo_Metallic);
        g_NormalsTarget[pixel] = float4(NormalToOctahedronUv(primaryHit.worldNormal), roughness, 0.0f);
        g_SkipTarget[pixel] = float4(primaryHit.emission, 0.0f);

        if (Constants.fuse_mode == 2)
        {
            float3 specularRadiance = specularAlbedo_NoV.xyz * (primaryHit.specularRadiance + indirectSpecularSample);
            float3 diffuseRadiance = diffuseAlbedo_Metallic.xyz * (1.0f - material.metallic) * (primaryHit.diffuseRadiance + indirectDiffuseSample);
            float3 fusedRadiance = (specularRadiance + diffuseRadiance) / fusedModulator;
            
            g_DirectSpecularTarget[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
            g_DirectDiffuseTarget[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
            g_IndirectSpecularTarget[pixel] = float4(fusedRadiance, 0.0f);
            g_IndirectDiffuseTarget[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
            g_FusedAlbedoTarget[pixel] = sqrt(float4(fusedModulator, specularAlbedo_NoV.w));
        }
        else if (Constants.fuse_mode == 1)
        {
            float3 fusedSpecularRadiance = primaryHit.specularRadiance + indirectSpecularSample;
            float3 fusedDiffuseRadiance = primaryHit.diffuseRadiance + indirectDiffuseSample;
            
            g_DirectSpecularTarget[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
            g_DirectDiffuseTarget[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
            g_IndirectSpecularTarget[pixel] = float4(fusedSpecularRadiance, 0.0f);
            g_IndirectDiffuseTarget[pixel] = float4(fusedDiffuseRadiance, 0.0f);
            g_FusedAlbedoTarget[pixel] = float4(0, 0, 0, 0);
        }
        else
        {
            g_DirectSpecularTarget[pixel] = float4(primaryHit.specularRadiance, 0.0f);
            g_DirectDiffuseTarget[pixel] = float4(primaryHit.diffuseRadiance, 0.0f);
            g_IndirectSpecularTarget[pixel] = float4(indirectSpecularSample, 0.0f);
            g_IndirectDiffuseTarget[pixel] = float4(indirectDiffuseSample, 0.0f);
            g_FusedAlbedoTarget[pixel] = float4(0, 0, 0, 0);
        }

        if (Constants.use_dominant_light)
        {
            g_DominantLightVisibilityTarget[pixel] = primaryHit.dominantLightVisibility * 65504.0f;
        }
        else
        {
            g_DominantLightVisibilityTarget[pixel] = 0.0f;
        }
    }
    else
    {
        g_DirectSpecularTarget[pixel] = float4(0,0,0,0);
        g_DirectDiffuseTarget[pixel] = float4(0,0,0,0);
        g_IndirectSpecularTarget[pixel] = float4(0,0,0,0);
        g_IndirectDiffuseTarget[pixel] = float4(0,0,0,0);
        g_DiffuseAlbedoTarget[pixel] = float4(0,0,0,0);
        g_SpecularAlbedoTarget[pixel] = float4(0,0,0,0);
        g_FusedAlbedoTarget[pixel] = float4(0,0,0,0);
        g_NormalsTarget[pixel] = float4(0,0,0,0);
        g_SkipTarget[pixel] = float4(primaryHit.radiance, 0.0f);
        g_DominantLightVisibilityTarget[pixel] = 0.0f;
    }
}
