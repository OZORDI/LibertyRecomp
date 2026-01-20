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

#ifndef IMPORTANCE_SAMPLING_H
#define IMPORTANCE_SAMPLING_H

#include "common.hlsl"

float3 SampleCone(inout uint rngState, float3 direction, float coneAngle)
{
    float cosAngle = cos(coneAngle);

    float z = RandomFloat01(rngState) * (1.0f - cosAngle) + cosAngle;
    float phi = RandomFloat01(rngState) * 2.0f * M_PI;

    float x = sqrt(1.0f - z * z) * cos(phi);
    float y = sqrt(1.0f - z * z) * sin(phi);
    float3 north = float3(0.0f, 0.0f, 1.0f);

    float3 axis = normalize(cross(north, direction));
    float angle = acos(dot(north, direction));

    float3x3 rotation = CreateRotation3x3(angle, axis);

    return mul(rotation, float3(x, y, z));
}

float2 ConcentricSampleDisk(inout uint rngState)
{
    float2 u = float2(RandomFloat01(rngState), RandomFloat01(rngState));
    float2 uOffset = 2.f * u - float2(1, 1);

    if (uOffset.x == 0 && uOffset.y == 0)
    {
        return float2(0, 0);
    }

    float theta, r;
    if (abs(uOffset.x) > abs(uOffset.y))
    {
        r = uOffset.x;
        theta = M_PI_OVER_FOUR * (uOffset.y / uOffset.x);
    }
    else
    {
        r = uOffset.y;
        theta = M_PI_OVER_TWO - M_PI_OVER_FOUR * (uOffset.x / uOffset.y);
    }
    return r * float2(cos(theta), sin(theta));
}

float3 CosineSampleHemisphere(inout uint rngState)
{
    float2 d = ConcentricSampleDisk(rngState);
    float z = sqrt(max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
    return float3(d.x, d.y, z);
}

float CosineSampleHemispherePDF(float3 normal, float3 newDirection)
{
    return min(max(dot(normal, newDirection), 0.0001f), 1.0f) * M_ONE_OVER_PI;
}

float D_GGX(float NeDotVe, float alphaRoughness)
{
    if (NeDotVe < 0.0f)
    {
        return 0.0f;
    }

    // Numerically stable form of Walter 2007 GGX NDF
    float eps = 1e-5f;
    float denom = (1.0f - NeDotVe * NeDotVe) / (alphaRoughness + eps) + NeDotVe * NeDotVe;
    return 1.0f / (M_PI * alphaRoughness * denom * denom);
}

float G_SmithJointGGX(float NdotL, float NdotV, float alphaRoughnessSq)
{
    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);
    float GGX = GGXV + GGXL;
    if (GGX > 0.0f)
    {
        return 0.5f / GGX;
    }
    return 0.0f;
}

float GGXBoundedVndfPdf(float3 Ve, float3 Ne, float alphaRoughness)
{
    float NeDotVe = min(max(dot(Ve, Ne), 0.0001f), 1.0f);
    float ndf = D_GGX(NeDotVe, alphaRoughness);
    float2 ai = alphaRoughness * Ve.xy;
    float len2 = dot(ai, ai);
    float t = sqrt(len2 + Ve.z * Ve.z);
    if (Ve.z >= 0.0f)
    {
        float a = saturate(min(alphaRoughness, alphaRoughness)); // Eq. 6
        float s = 1.0f + length(float2(Ve.x, Ve.y)); // Omit sgn for a <=1
        float a2 = a * a;
        float s2 = s * s;
        float k = (1.0f - a2) * s2 / (s2 + a2 * Ve.z * Ve.z); // Eq. 5
        return ndf / (2.0f * (k * Ve.z + t)); // Eq. 8 * || dm/do ||
    }
    // Numerically stable form of the previous PDF for i.z < 0
    return ndf * (t - Ve.z) / (2.0f * len2); // = Eq. 7 * || dm/do ||
}

float3 SampleVndfHemisphereBounded(float3 Ve, float alpha_x, float alpha_y, float U1, float U2)
{
    float3 Vh = normalize(float3(alpha_x * Ve.x, alpha_y * Ve.y, Ve.z));
    // Sample a spherical cap
    float phi = 2.0f * M_PI * U1;
    float a = saturate(min(alpha_x, alpha_y)); // Eq. 6
    float s = 1.0f + length(float2(Ve.x, Ve.y)); // Omit sgn for a <=1
    float a2 = a * a;
    float s2 = s * s;
    float k = (1.0f - a2) * s2 / (s2 + a2 * Ve.z * Ve.z); // Eq. 5
    float b = Ve.z > 0 ? k * Vh.z : Vh.z;
    float z = mad(1.0f - U2, 1.0f + b, -b);
    float sinTheta = sqrt(saturate(1.0f - z * z));
    float3 o_std = { sinTheta * cos(phi), sinTheta * sin(phi), z };
    // Compute the microfacet normal m
    float3 m_std = Vh + o_std;
    float3 m = normalize(float3(m_std.xy * float2(alpha_x, alpha_y), m_std.z));
    // The reflection vector o
    float3 o = 2.0f * dot(Ve, m) * m - Ve;
    return m;
}

float3 SampleReflectionVector(inout uint rngState, float3 V, float3 N, float roughness)
{
    const float2 xi = float2(RandomFloat01(rngState), RandomFloat01(rngState));
    
    if (roughness < EPSILON)
    {
        return reflect(-V, N);
    }
    
    float3x3 TBN = CreateTBN(N);
    float3 TBN_V = mul(TBN, V);
    const float alpha = roughness * roughness;
    float3 TBN_N = SampleVndfHemisphereBounded(TBN_V, alpha, alpha, xi.x, xi.y);
    float3 TBN_Rd = reflect(-TBN_V, TBN_N);
    return normalize(mul(TBN_Rd, TBN));
}

float3 SampleReflectionVectorReroll(inout uint rngState, float3 V, float3 N, float roughness)
{
    float3 rd = SampleReflectionVector(rngState, V, N, roughness);
    for (uint i = 0; i < 4; i++)
    {
        if (dot(rd, N) > 0.0f)
            break;
        rd = SampleReflectionVector(rngState, V, N, roughness);
    }
    return rd;
}

#endif  // IMPORTANCE_SAMPLING_H
