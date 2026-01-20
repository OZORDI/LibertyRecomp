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

#ifndef SOURCE_BSDF_PERFECT
#define SOURCE_BSDF_PERFECT

#include "bsdf/BSDF.hlsli"

float SamplePerfectSpecular(float3 i, float3 n, out float3 o)
{
    o = reflect(-i, n);
    return kBxDFMaxPDF;
}

float SamplePerfectDielectric(float xi, float3 i, float3 n, float2 eta, out float3 o, inout float kickoff)
{    
    // Calculate the Fresnel coefficient and associated vectors. 
    float F = Fresnel(dot(i, n), eta.x, eta.y);       
    if(xi > F)
    {
        o = refract(-i, n, eta.x / eta.y);
        kickoff *= -1.;
    }
    else
    {
       o = reflect(-i, n);
    }
    return kBxDFMaxPDF;
}

float SampleLambertian(float2 xi, float3 n, out float3 o)
{        
    // Sample the Lambertian direction
    float3 r = float3(SampleUnitDisc(xi), 0.0f);
    r.z = sqrt(1.0 - sqr(r.x) - sqr(r.y));

    // Transform it to world space
    o = mul(CreateBasis(n), r);
    return r.z / kPi;
}

float EvaluateLambertian()
{
    return 1. / kPi;
}

#endif
