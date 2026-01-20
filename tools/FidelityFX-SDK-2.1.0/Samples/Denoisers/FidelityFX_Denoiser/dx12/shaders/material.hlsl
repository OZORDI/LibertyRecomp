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

#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.hlsl"

PBRPixelInfo GetPBRPixelInfoFromMaterial(Texture2D textures[], SamplerState samplers[], in LocalBasis local_basis, in PTMaterialInfo material, float mipLevel)
{
    PBRPixelInfo pbrPixelInfo = (PBRPixelInfo)0;

    float3 normal = local_basis.normal;
    float3 tangent = local_basis.tangent.xyz;
    if (material.normal_tex_id >= 0 && any(abs(tangent) > 0.0f))
    {
        tangent = normalize(tangent);
        float3 binormal = normalize(cross(normal, tangent)) * local_basis.tangent.w;
        const float2 xy = textures[NonUniformResourceIndex(material.normal_tex_id)].SampleLevel(samplers[material.normal_tex_sampler_id], local_basis.uv, mipLevel).rg;
        const float z   = sqrt(1.0f - saturate(dot(xy, xy)));
        normal = normalize(z * normal + (2.0f * xy.x - 1.0f) * tangent + -(2.0f * xy.y - 1.0f) * binormal);
    }
    pbrPixelInfo.pixelBaseColorAlpha = float4(material.albedo_factor_x, material.albedo_factor_y, material.albedo_factor_z, material.albedo_factor_w);
    if (material.albedo_tex_id >= 0)
    {
        pbrPixelInfo.pixelBaseColorAlpha = pbrPixelInfo.pixelBaseColorAlpha * textures[NonUniformResourceIndex(material.albedo_tex_id)].SampleLevel(samplers[material.albedo_tex_sampler_id], local_basis.uv, mipLevel);
    }
    float3 arm = float3(1.0f, material.arm_factor_y, material.arm_factor_z);
    if (material.arm_tex_id >= 0)
    {
        arm = arm * textures[NonUniformResourceIndex(material.arm_tex_id)].SampleLevel(samplers[material.arm_tex_sampler_id], local_basis.uv, mipLevel).xyz;
    }

    pbrPixelInfo.pixelNormal = float4(normal, 0.f);
    pbrPixelInfo.pixelAoRoughnessMetallic = saturate(float3(arm.r, arm.g, arm.b));

    return pbrPixelInfo;
}

MaterialInfo GetMaterialInfo(PBRPixelInfo pbrPixelInfo)
{
    MaterialInfo materialInfo = (MaterialInfo)0;

    materialInfo.perceptualRoughness = max(pbrPixelInfo.pixelAoRoughnessMetallic.g, EPSILON);
    materialInfo.alphaRoughness = max(pbrPixelInfo.pixelAoRoughnessMetallic.g * pbrPixelInfo.pixelAoRoughnessMetallic.g, EPSILON);
    materialInfo.metallic = pbrPixelInfo.pixelAoRoughnessMetallic.b;
    materialInfo.baseColor = pbrPixelInfo.pixelBaseColorAlpha.rgb * (1.0 - pbrPixelInfo.pixelAoRoughnessMetallic.b);
    float3 metallic = float3(pbrPixelInfo.pixelAoRoughnessMetallic.b, pbrPixelInfo.pixelAoRoughnessMetallic.b, pbrPixelInfo.pixelAoRoughnessMetallic.b);
    materialInfo.reflectance0 = lerp(float3(MinReflectance, MinReflectance, MinReflectance), pbrPixelInfo.pixelBaseColorAlpha.rgb, metallic);
    materialInfo.reflectance90 = float3(1.0, 1.0, 1.0);

    return materialInfo;
}

#endif  // MATERIAL_H
