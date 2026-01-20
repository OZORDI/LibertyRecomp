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

#include "commonintersect.hlsl"

#define USE_INFINITE_FAR_PLANE

Texture2D<float> g_DepthTarget : register(t0);
Texture2D<float4> g_GBufferMotionVectors : register(t1);

RWTexture2D<float> g_LinearDepth : register(u0);
RWTexture2D<float4> g_MotionVectors : register(u1);

RWTexture2D<float4> g_DenoisedDirectDiffuse : register(u2);
RWTexture2D<float4> g_DenoisedDirectSpecular : register(u3);
RWTexture2D<float4> g_DenoisedIndirectDiffuse : register(u4);
RWTexture2D<float4> g_DenoisedIndirectSpecular : register(u5);
RWTexture2D<float4> g_DenoisedDominantLightVisibility : register(u6);

cbuffer Constants : register(b0)
{
    float4x4 g_ClipToCamera;
    float4x4 g_ClipToWorld;
    float4x4 g_PrevWorldToCamera;
    float g_RenderWidth;
    float g_RenderHeight;
    float g_CameraNear;
    float g_CameraFar;
};

[numthreads(8, 8, 1)]
void main(uint3 dtid : SV_DispatchThreadID)
{
    uint2 pixel = dtid.xy;
    if (pixel.x < g_RenderWidth && pixel.y < g_RenderHeight)
    {
        float depth = g_DepthTarget[pixel];
        float3 screenUVW = float3((float2(pixel) + 0.5f) / float2(g_RenderWidth, g_RenderHeight), depth);
        float3 farViewSpacePos = ScreenSpaceToViewSpace(float3(screenUVW.xy, 0.0f), g_ClipToCamera);
        float3 viewSpacePos = ScreenSpaceToViewSpace(screenUVW, g_ClipToCamera);
        float3 worldSpacePos = ScreenSpaceToWorldSpace(screenUVW, g_ClipToWorld);
        float3 prevViewSpacePos = mul(g_PrevWorldToCamera, float4(worldSpacePos, 1.0f)).xyz;
    #ifdef USE_INFINITE_FAR_PLANE
        float farZ = (farViewSpacePos.z > 0.0f) ? g_CameraFar : -g_CameraFar; // The sdk is using infinite far plane, so clamp it to a finite value here.
        viewSpacePos.z = depth > 0.0f ? viewSpacePos.z : farZ;
    #endif
    
        g_LinearDepth[pixel] = -viewSpacePos.z;
    
        float depthDiff = (prevViewSpacePos.z - viewSpacePos.z);
        float2 mv = g_GBufferMotionVectors[pixel].xy;
        float3 motionVector = float3(mv, depthDiff);
        g_MotionVectors[pixel] = float4(motionVector, 0.0f);
    }
    else
    {
        g_LinearDepth[pixel] = 0.0f;
        g_MotionVectors[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    // clear outputs to validate that history is kept by denoiser internally
    g_DenoisedDirectDiffuse[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    g_DenoisedDirectSpecular[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    g_DenoisedIndirectDiffuse[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    g_DenoisedIndirectSpecular[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    g_DenoisedDominantLightVisibility[pixel] = float4(0.0f, 0.0f, 0.0f, 0.0f);
}
