/*******************************************************************************
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * Intel XeSS SDK - D3D12 API Header
 * Based on Intel XeSS SDK 2.1
 *
 * For full SDK, download from: https://github.com/intel/xess/releases
 ******************************************************************************/

#ifndef XESS_D3D12_H
#define XESS_D3D12_H

#include "xess.h"

#ifdef _WIN32
#include <d3d12.h>

#ifdef __cplusplus
extern "C" {
#endif

// D3D12 initialization parameters
typedef struct xess_d3d12_init_params_t {
    xess_2d_t outputResolution;
    xess_quality_settings_t qualitySetting;
    xess_init_flags_t initFlags;
    xess_network_model_t networkModel;
    ID3D12PipelineLibrary* pPipelineLibrary;
    ID3D12Heap* pTempTextureHeap;
    uint32_t bufferHeapOffset;
    ID3D12Heap* pTempBufferHeap;
    uint32_t textureHeapOffset;
    uint32_t creationNodeMask;
    uint32_t visibleNodeMask;
} xess_d3d12_init_params_t;

// D3D12 execution parameters
typedef struct xess_d3d12_execute_params_t {
    ID3D12Resource* pColorTexture;
    ID3D12Resource* pVelocityTexture;
    ID3D12Resource* pDepthTexture;
    ID3D12Resource* pExposureScaleTexture;
    ID3D12Resource* pResponsivePixelMaskTexture;
    ID3D12Resource* pOutputTexture;
    float jitterOffsetX;
    float jitterOffsetY;
    xess_2d_t inputResolution;
    float exposureScale;
    uint32_t resetHistory;
    uint32_t frameIndex;
} xess_d3d12_execute_params_t;

// D3D12 function pointer types
typedef xess_result_t (*PFN_xessD3D12CreateContext)(ID3D12Device* pDevice, xess_context_handle_t* phContext);
typedef xess_result_t (*PFN_xessD3D12Init)(xess_context_handle_t hContext, const xess_d3d12_init_params_t* pInitParams);
typedef xess_result_t (*PFN_xessD3D12Execute)(xess_context_handle_t hContext, ID3D12GraphicsCommandList* pCommandList, const xess_d3d12_execute_params_t* pExecParams);
typedef xess_result_t (*PFN_xessD3D12BuildPipelines)(xess_context_handle_t hContext, ID3D12PipelineLibrary* pPipelineLibrary, uint32_t blocking, uint32_t initFlags);
typedef xess_result_t (*PFN_xessD3D12GetResourcesToBuild)(xess_context_handle_t hContext, uint32_t* pNumPipelines, uint32_t initFlags);

#ifdef __cplusplus
}
#endif

#endif // _WIN32

#endif // XESS_D3D12_H
