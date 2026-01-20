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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "common.hlsl"

uint3 FetchFaceIndicesU32(StructuredBuffer<uint> indexBuffers[], in uint bufferIndex, in uint triangleId)
{
    uint3 face3;
    face3[0] = indexBuffers[NonUniformResourceIndex(bufferIndex)].Load(3 * triangleId);
    face3[1] = indexBuffers[NonUniformResourceIndex(bufferIndex)].Load(3 * triangleId + 1);
    face3[2] = indexBuffers[NonUniformResourceIndex(bufferIndex)].Load(3 * triangleId + 2);
    return face3;
}

uint3 FetchFaceIndicesU16(StructuredBuffer<uint> indexBuffers[], in uint bufferIndex, in uint triangleId)
{
    uint word_id_0  = triangleId * 3 + 0;
    uint dword_id_0 = word_id_0 / 2;
    uint shift_0    = 16 * (word_id_0 & 1);

    uint word_id_1  = triangleId * 3 + 1;
    uint dword_id_1 = word_id_1 / 2;
    uint shift_1    = 16 * (word_id_1 & 1);

    uint word_id_2  = triangleId * 3 + 2;
    uint dword_id_2 = word_id_2 / 2;
    uint shift_2    = 16 * (word_id_2 & 1);

    uint u0 = indexBuffers[NonUniformResourceIndex(bufferIndex)].Load(dword_id_0);
    u0      = (u0 >> shift_0) & 0xffffu;
    uint u1 = indexBuffers[NonUniformResourceIndex(bufferIndex)].Load(dword_id_1);
    u1      = (u1 >> shift_1) & 0xffffu;
    uint u2 = indexBuffers[NonUniformResourceIndex(bufferIndex)].Load(dword_id_2);
    u2      = (u2 >> shift_0) & 0xffffu;
    return uint3(u0, u1, u2);
}

float FetchVertexFloat(StructuredBuffer<float> vertexBuffers[], in int offset, in int vertexId)
{
    return vertexBuffers[NonUniformResourceIndex(offset)].Load(vertexId);
}

float2 FetchVertexFloat2(StructuredBuffer<float> vertexBuffers[], in int offset, in int vertexId)
{
    float2 data;
    data[0] = vertexBuffers[NonUniformResourceIndex(offset)].Load(2 * vertexId);
    data[1] = vertexBuffers[NonUniformResourceIndex(offset)].Load(2 * vertexId + 1);

    return data;
}
float3 FetchVertexFloat3(StructuredBuffer<float> vertexBuffers[], in int offset, in int vertexId)
{
    float3 data;
    data[0] = vertexBuffers[NonUniformResourceIndex(offset)].Load(3 * vertexId);
    data[1] = vertexBuffers[NonUniformResourceIndex(offset)].Load(3 * vertexId + 1);
    data[2] = vertexBuffers[NonUniformResourceIndex(offset)].Load(3 * vertexId + 2);

    return data;
}
float4 FetchVertexFloat4(StructuredBuffer<float> vertexBuffers[], in int offset, in int vertexId)
{
    float4 data;
    data[0] = vertexBuffers[NonUniformResourceIndex(offset)].Load(4 * vertexId);
    data[1] = vertexBuffers[NonUniformResourceIndex(offset)].Load(4 * vertexId + 1);
    data[2] = vertexBuffers[NonUniformResourceIndex(offset)].Load(4 * vertexId + 2);
    data[3] = vertexBuffers[NonUniformResourceIndex(offset)].Load(4 * vertexId + 3);

    return data;
}

void FetchVertexAttributes(StructuredBuffer<float> vertexBuffers[], in PTSurfaceInfo surfaceInfo, in uint vertexOffset, out float3 position, out float2 uv, out float3 normal, out float4 tangent)
{
    normal = FetchVertexFloat3(vertexBuffers, surfaceInfo.normal_attribute_offset, vertexOffset);
    
    if (surfaceInfo.tangent_attribute_offset >= 0)
    {
        tangent = FetchVertexFloat4(vertexBuffers, surfaceInfo.tangent_attribute_offset, vertexOffset);
    }
    else
    {
        tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    if (surfaceInfo.texcoord0_attribute_offset >= 0)
    {
        uv = FetchVertexFloat2(vertexBuffers, surfaceInfo.texcoord0_attribute_offset, vertexOffset);
    }
    else
    {
        uv = float2(0.0f, 0.0f);
    }
    
    if (surfaceInfo.position_attribute_offset >= 0)
    {
        position = FetchVertexFloat3(vertexBuffers, surfaceInfo.position_attribute_offset, vertexOffset);
    }
    else
    {
        position = float3(0.0f, 0.0f, 0.0f);
    }
}

LocalBasis FetchLocalBasis(StructuredBuffer<float> vertexBuffers[], in PTSurfaceInfo surfaceInfo, float3x3 objectToWorld, in uint3 face3, in float2 barycentrics, bool isFrontFace)
{
    const float3 fullBary = float3(1.0f - barycentrics.x - barycentrics.y, barycentrics.xy);

    float3 positions[3];
    float3 normals[3];
    float4 tangent[3];
    float2 uvs[3];
    for (uint i = 0; i < 3; ++i)
    {
        FetchVertexAttributes(vertexBuffers, surfaceInfo, face3[i], positions[i], uvs[i], normals[i], tangent[i]);
    }

    const float3 uv0 = float3(uvs[0], 0.0f);
    const float3 uv1 = float3(uvs[1], 0.0f);
    const float3 uv2 = float3(uvs[2], 0.0f);

    LocalBasis localBasis = (LocalBasis) 0;
    localBasis.uv = uvs[0] * fullBary.x + uvs[1] * fullBary.y + uvs[2] * fullBary.z;
    localBasis.uvArea = abs(cross(uv1 - uv0, uv2 - uv0).z);
    localBasis.normal = normals[0] * fullBary.x + normals[1] * fullBary.y + normals[2] * fullBary.z;
    localBasis.triangleArea = length(cross(positions[1] - positions[0], positions[2] - positions[0])) * 0.5f;
    localBasis.tangent = tangent[0] * fullBary.x + tangent[1] * fullBary.y + tangent[2] * fullBary.z;

    localBasis.normal = mul(objectToWorld, localBasis.normal);
    localBasis.tangent.xyz = mul(objectToWorld, localBasis.tangent.xyz);

    if (!isFrontFace)
    {
        localBasis.normal = -localBasis.normal;
    }

    localBasis.normal = normalize(localBasis.normal);
    localBasis.tangent.xyz = normalize(localBasis.tangent.xyz);
    
    return localBasis;
}
#endif  // GEOMETRY_H
