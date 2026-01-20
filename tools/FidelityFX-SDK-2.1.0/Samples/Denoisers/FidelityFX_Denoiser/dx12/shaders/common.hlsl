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

#ifndef COMMON_H
#define COMMON_H

#include "types.hlsl"
#include "lightfunctionscommon.hlsl"
#include "commonintersect.hlsl"

static const float M_ONE_OVER_PI = (1.0f / M_PI);
static const float M_ONE_OVER_TWO_PI = (1.0f / (2.0f * M_PI));
static const float M_PI_OVER_TWO = (M_PI / 2.0f);
static const float M_PI_OVER_FOUR = (M_PI / 4.0f);
static const float EPSILON = 0.0001f;
static const float FLT_MAX = 3.402823466e+38f;

#include "random.hlsl"

float Square(float x) { return x*x; }
float2 Square(float2 x) { return x*x; }
float3 Square(float3 x) { return x*x; }
float4 Square(float4 x) { return x*x; }

float3x3 CreateRotation3x3(float radAngle, float3 axis)
{
    float c, s;
    sincos(radAngle, s, c);

    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return float3x3(t * x * x + c,
                    t * x * y - s * z,
                    t * x * z + s * y,
                    t * x * y + s * z,
                    t * y * y + c,
                    t * y * z - s * x,
                    t * x * z - s * y,
                    t * y * z + s * x,
                    t * z * z + c);
}

float3 RotateVector(float4x4 mat, float3 v)
{
    return mul(float3x3(mat[0].xyz, mat[1].xyz, mat[2].xyz), v);
}

float3 RotateVector(float3x4 mat, float3 v)
{
    return mul(float3x3(mat[0].xyz, mat[1].xyz, mat[2].xyz), v);
}

float3 RotateVector(float3x3 mat, float3 v)
{
    return mul(float3x3(mat[0].xyz, mat[1].xyz, mat[2].xyz), v);
}

float3x3 CreateTBN(float3 N)
{
    float3 T;
    if (abs(N.z) > 0.0f)
    {
        float k = sqrt(N.y * N.y + N.z * N.z);
        T.x = 0.0f;
        T.y = -N.z / k;
        T.z = N.y / k;
    }
    else
    {
        float k = sqrt(N.x * N.x + N.y * N.y);
        T.x = N.y / k;
        T.y = -N.x / k;
        T.z = 0.0f;
    }
    float3x3 TBN;
    TBN[0] = T;
    TBN[1] = cross(N, T);
    TBN[2] = N;
    return TBN;
}

float2 NormalToOctahedronUv(float3 N)
{
    N.xy = float2(N.xy) / (abs(N.x) + abs(N.y) + abs(N.z));
    float2 k;
    k.x = N.x > 0.0f ? 1.0f : -1.0f;
    k.y = N.y > 0.0f ? 1.0f : -1.0f;
    if (N.z < 0.0f)
        N.xy = (1.0f - abs(float2(N.yx))) * k;
    return N.xy * 0.5f + 0.5f;
}

float3 OctahedronUvToNormal(float2 UV)
{
    UV = 2.0f * (UV - 0.5f);
    float3 N = float3(UV, 1.0f - abs(UV.x) - abs(UV.y));
    float t = max(-N.z, 0.0f);
    float2 k;
    k.x = N.x >= 0.0f ? -t : t;
    k.y = N.y >= 0.0f ? -t : t;
    N.xy += k;
    return normalize(N);
}

void BranchlessONB(float3 n, inout float4 t, inout float3 b)
{
    float sign = n.z >= 0 ? 1.0f : -1.0f;
    const float x = -1.0f / (sign + n.z);
    const float y = n.x * n.y * x;
    t = float4(1.0f + sign * n.x * n.x * x, sign * y, -sign * n.x, 1);
    b = float3(y, sign + n.y * n.y * x, -n.y);
}

#endif  // COMMON_H
