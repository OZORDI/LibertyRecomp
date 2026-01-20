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

#ifndef TYPES_H
#define TYPES_H

#include "shared.h"
#include "shadercommon.h"

struct LocalBasis
{
    float2 uv;
    float3 normal;
    float4 tangent;
    float triangleArea;
    float uvArea;
};

struct TraceRayDesc
{
    float3 origin;
    float3 direction;
    float tMin;
    float tMax;
    uint recursionIndex;
};

struct TraceRayHitInfo
{
    float3x3 objectToWorld;
    float3 position;
    float2 barycentrics;
    float rayT;
    uint instanceId;
    uint geometryIndex;
    uint triangleId;
    bool isFrontFace;
};

struct TraceRayHitResult
{
    bool hit;
    float3 radiance;
    float3 diffuseRadiance;
    float3 specularRadiance;
    float3 emission;

    MaterialInfo materialInfo;
    LocalBasis localBasis;
    float3 worldPosition;
    float3 worldNormal;
    float dominantLightVisibility;
};

#endif  // TYPES_H
