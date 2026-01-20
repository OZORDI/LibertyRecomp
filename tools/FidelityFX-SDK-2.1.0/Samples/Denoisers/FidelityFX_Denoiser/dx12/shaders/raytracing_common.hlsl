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

#ifndef RAYTRACING_COMMON_H
#define RAYTRACING_COMMON_H

#include "common.hlsl"
#include "importance_sampling.hlsl"
#include "geometry.hlsl"
#include "material.hlsl"

float CalculateMipLevelFromRayHit(float rayT, LocalBasis localBasis)
{
    float footprint = localBasis.uvArea / (localBasis.triangleArea / (rayT * rayT));
    float lambda = 0.5f * log2(footprint);
    return max(0.0f, lambda);
}

float TraceShadowRay(RaytracingAccelerationStructure rtas, float3 origin, float3 direction, float tMin = 0.01f, float tMax = 1024.0f)
{
    RayDesc shadowRay;
    shadowRay.Origin = origin;
    shadowRay.Direction = direction;
    shadowRay.TMin = tMin;
    shadowRay.TMax = tMax;

    RayQuery<RAY_FLAG_CULL_NON_OPAQUE | RAY_FLAG_SKIP_PROCEDURAL_PRIMITIVES | RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER> shadowQuery;
    shadowQuery.TraceRayInline(rtas, 0, 0xff, shadowRay);
    shadowQuery.Proceed();
    return shadowQuery.CommittedStatus() == COMMITTED_TRIANGLE_HIT ? 0.0f : 1.0f;
}

float GetSoftShadowFactor(RaytracingAccelerationStructure rtas, LightInformation lightInfo, inout uint rngState, float3 worldPosition, float3 normal, uint numSamples)
{
    const float lightSourceRadius = 0.1f;
    const float sampleWeight = 1.0f / float(numSamples);
    const float3 rayOrigin = worldPosition + normal * 3.0e-3;
    float shadowFactor = 0.0f;
    if (lightInfo.Type == 0 /*LightType::Directional*/)
    {
        const float3 rayDirection = lightInfo.DirectionRange.xyz;
        for (int rayIdx = 0; rayIdx < numSamples; ++rayIdx)
        {
            float2 diskSample = ConcentricSampleDisk(rngState) * lightSourceRadius;
            float3 offset = (diskSample.x * normalize(cross(normal, rayDirection))) + diskSample.y * normalize(cross(cross(normal, rayDirection), normal));
            float3 offsettedOrigin = rayOrigin + offset;
            shadowFactor += TraceShadowRay(rtas, offsettedOrigin, rayDirection) * sampleWeight;
        }
    }
    else if (lightInfo.Type == 1 /*LightType::Spot*/ || lightInfo.Type == 2 /*LightType::Point*/)
    {
        for (int rayIdx = 0; rayIdx < numSamples; ++rayIdx)
        {
            float3 lightPos = lightInfo.PosDepthBias.xyz;
            float3 toLight = lightPos - worldPosition;
            float distanceToLight = length(toLight);
            toLight = normalize(toLight);
            
            float3 perpL = cross(toLight, float3(0.0f, 1.0f, 0.0f));
            perpL = (all(perpL == 0.0f)) ? 1.0f : perpL;

            float3 toLightEdge = normalize((lightPos + perpL * lightSourceRadius) - worldPosition);
            float coneAngle = acos(dot(toLight, toLightEdge)) * 2.0f;
            float3 rayDirection = SampleCone(rngState, toLight, coneAngle);
            shadowFactor += TraceShadowRay(rtas, rayOrigin, rayDirection, 0.01f, distanceToLight) * sampleWeight;
        }
    }
    return shadowFactor;
}

float GetShadowFactor(RaytracingAccelerationStructure rtas, LightInformation lightInfo, inout uint rngState, float3 worldPosition, float3 normal, bool useSoftShadows = false, uint numSamples = 1)
{
    if (useSoftShadows)
        return GetSoftShadowFactor(rtas, lightInfo, rngState, worldPosition, normal, numSamples);
    else
    {
        const float3 rayOrigin = worldPosition + normal * 3.0e-3;
        if (lightInfo.Type == 0 /*LightType::Directional*/)
        {
            float3 rayDirection = lightInfo.DirectionRange.xyz;
            return TraceShadowRay(rtas, rayOrigin, rayDirection);
        }
        else if (lightInfo.Type == 1 /*LightType::Spot*/ || lightInfo.Type == 2 /*LightType::Point*/)
        {
            float3 rayDirection = lightInfo.PosDepthBias.xyz - worldPosition;
            float distanceToLight = length(rayDirection);
            rayDirection = normalize(rayDirection);
            return TraceShadowRay(rtas, rayOrigin, rayDirection, 0.01f, distanceToLight);
        }
        
        return 0.0f;
    }
}

void TraceRay_OnHit(inout TraceRayHitResult result, in TraceRayDesc ray, in TraceRayHitInfo hitInfo, inout uint rngState);
void TraceRay_OnMiss(inout TraceRayHitResult result, in TraceRayDesc ray, inout uint rngState);
TraceRayHitResult TraceRay(RaytracingAccelerationStructure rtas, in TraceRayDesc ray, inout uint rngState)
{
    RayDesc rayDesc;
    rayDesc.TMin = ray.tMin;
    rayDesc.TMax = ray.tMax;
    rayDesc.Origin = ray.origin;
    rayDesc.Direction = ray.direction;

    RayQuery<RAY_FLAG_CULL_NON_OPAQUE | RAY_FLAG_SKIP_PROCEDURAL_PRIMITIVES> rayQuery;
    rayQuery.TraceRayInline(rtas, 0, 0xff, rayDesc);
    rayQuery.Proceed();
    
    TraceRayHitResult result = (TraceRayHitResult)0;
    if (rayQuery.CommittedStatus() == COMMITTED_TRIANGLE_HIT)
    {
        TraceRayHitInfo hitInfo;
        
        hitInfo.position = ray.origin + ray.direction * rayQuery.CommittedRayT();
        hitInfo.instanceId = rayQuery.CommittedInstanceID();
        hitInfo.geometryIndex = rayQuery.CommittedGeometryIndex();
        hitInfo.triangleId = rayQuery.CommittedPrimitiveIndex();
        hitInfo.barycentrics = rayQuery.CommittedTriangleBarycentrics();
        hitInfo.objectToWorld = float3x3(rayQuery.CommittedObjectToWorld3x4()[0].xyz, rayQuery.CommittedObjectToWorld3x4()[1].xyz, rayQuery.CommittedObjectToWorld3x4()[2].xyz);
        hitInfo.isFrontFace = rayQuery.CommittedTriangleFrontFace();
        hitInfo.rayT = rayQuery.CommittedRayT();
        
        TraceRay_OnHit(result, ray, hitInfo, rngState);
        result.hit = true;
    }
    else
    {
        TraceRay_OnMiss(result, ray, rngState);
        result.hit = false;
    }
    
    return result;
}

SplitShadingResult Shade(float3 pointToLight, MaterialInfo materialInfo, float3 normal, float3 view)
{
    AngularInfo angularInfo = GetAngularInfo(pointToLight, normal, view);

    SplitShadingResult result = (SplitShadingResult)0;
    if (angularInfo.NdotL > 0.0 || angularInfo.NdotV > 0.0)
    {
        result.diffuse = angularInfo.NdotL;
        
        float Vis = VisibilityOcclusion_SmithJointGGX(materialInfo, angularInfo);
        float D = MicrofacetDistribution_Trowbridge(materialInfo, angularInfo);
        result.specular = Vis * D * angularInfo.NdotL;
    }

    return result;
}

LightingResult DirectionalLight(LightInformation light, MaterialInfo materialInfo, float3 normal, float3 view)
{
    float3 pointToLight = light.DirectionRange.xyz;

    float3 lightIntensity = light.ColorIntensity.rgb * light.ColorIntensity.a;
    SplitShadingResult shadingResult = Shade(pointToLight, materialInfo, normal, view);
    
    LightingResult lightingResult = (LightingResult)0;
    lightingResult.diffuse = lightIntensity * shadingResult.diffuse;
    lightingResult.specular = lightIntensity * shadingResult.specular;
    return lightingResult;
}

LightingResult PointLight(LightInformation light, MaterialInfo materialInfo, float3 normal, float3 worldPos, float3 view)
{
    float3 pointToLight = light.PosDepthBias.xyz - worldPos;
    float distance = length(pointToLight);

    // The GLTF attenuation function expects a range, make this happen CPU side?
    light.DirectionRange.w = light.DirectionRange.w > 0 ? light.DirectionRange.w : LIGHT_MAX_RANGE;

    // Early out if we're out of range of the light.
    if (distance > light.DirectionRange.w)
    {
        return (LightingResult)0;
    }

    float attenuation = GetRangeAttenuation(light.DirectionRange.w, distance);
    float3 lightIntensity = attenuation * light.ColorIntensity.rgb * light.ColorIntensity.a;
    SplitShadingResult shadingResult = Shade(pointToLight, materialInfo, normal, view);
    
    LightingResult lightingResult = (LightingResult)0;
    lightingResult.diffuse = lightIntensity * shadingResult.diffuse;
    lightingResult.specular = lightIntensity * shadingResult.specular;
    return lightingResult;
}

LightingResult SpotLight(LightInformation light, MaterialInfo materialInfo, float3 normal, float3 worldPos, float3 view)
{
    float3 pointToLight = light.PosDepthBias.xyz - worldPos;
    float distance = length(pointToLight);

    // The GLTF attenuation function expects a range, make this happen CPU side?
    light.DirectionRange.w = light.DirectionRange.w > 0 ? light.DirectionRange.w : LIGHT_MAX_RANGE;

#if (DEF_doubleSided == 1)
    if (dot(normal, pointToLight) < 0)
    {
        normal = -normal;
    }
#endif

    float rangeAttenuation = GetRangeAttenuation(light.DirectionRange.w, distance);
    float spotAttenuation = GetSpotAttenuation(pointToLight, -light.DirectionRange.xyz, light.OuterConeCos, light.InnerConeCos);
    float3 lightIntensity = rangeAttenuation * spotAttenuation * light.ColorIntensity.rgb * light.ColorIntensity.a;
    SplitShadingResult shadingResult = Shade(pointToLight, materialInfo, normal, view);
    
    LightingResult lightingResult = (LightingResult)0;
    lightingResult.diffuse = lightIntensity * shadingResult.diffuse;
    lightingResult.specular = lightIntensity * shadingResult.specular;
    return lightingResult;
}

LightingResult ApplyLight(in float3 worldPos, in float3 normal, in float3 view, in MaterialInfo materialInfo, in LightInformation lightInfo)
{
    LightingResult result = (LightingResult)0;

    if (lightInfo.Type == 0 /* LightType::Directional */)
    {
        result = DirectionalLight(lightInfo, materialInfo, normal, view);
    }
    else if (lightInfo.Type == 2 /* LightType::Point */)
    {
        result = PointLight(lightInfo, materialInfo, normal, worldPos, view);
    }
    else if (lightInfo.Type == 1 /* LightType::Spot */)
    {
        result = SpotLight(lightInfo, materialInfo, normal, worldPos, view);
    }

    return result;
}

#endif // RAYTRACING_COMMON_H
