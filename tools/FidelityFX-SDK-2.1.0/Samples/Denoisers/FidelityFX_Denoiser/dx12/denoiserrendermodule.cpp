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

#include "denoiserrendermodule.h"

#include "Cauldron2/dx12/framework/core/framework.h"
#include "Cauldron2/dx12/framework/core/contentmanager.h"
#include "Cauldron2/dx12/framework/core/scene.h"
#include "Cauldron2/dx12/framework/core/inputmanager.h"
#include "Cauldron2/dx12/framework/core/backend_interface.h"
#include "Cauldron2/dx12/framework/core/components/meshcomponent.h"
#include "Cauldron2/dx12/framework/render/dynamicresourcepool.h"
#include "Cauldron2/dx12/framework/render/dynamicbufferpool.h"
#include "Cauldron2/dx12/framework/render/parameterset.h"
#include "Cauldron2/dx12/framework/render/pipelineobject.h"
#include "Cauldron2/dx12/framework/render/indirectworkload.h"
#include "Cauldron2/dx12/framework/render/rasterview.h"
#include "Cauldron2/dx12/framework/render/profiler.h"
#include "Cauldron2/dx12/framework/render/commandlist.h"
#include "Cauldron2/dx12/framework/shaders/lightingcommon.h"

#include "Cauldron2/dx12/framework/render/dx12/commandlist_dx12.h"

#include <FidelityFX/api/include/dx12/ffx_api_dx12.hpp>
#include <Cauldron2/dx12/framework/render/dx12/device_dx12.h>

using namespace cauldron;

DenoiserRenderModule::~DenoiserRenderModule()
{
    DestroyContext();
}

void DenoiserRenderModule::Init(const json& initData)
{
    CauldronAssert(ASSERT_CRITICAL, GetFramework()->GetConfig()->MinShaderModel >= ShaderModel::SM6_6, L"Error: Denoiser requires SM6_6 or greater");

    InitPipelineObjects();
    InitResources();

    // Query Denoiser versions
    {
        uint64_t DenoiserVersionCount = 0;
        ffx::QueryDescGetVersions queryVersionsDesc = {};
        queryVersionsDesc.createDescType = FFX_API_EFFECT_ID_DENOISER;
        queryVersionsDesc.device = GetDevice()->GetImpl()->DX12Device();
        queryVersionsDesc.outputCount = &DenoiserVersionCount;
        ffx::Query(queryVersionsDesc);

        m_DenoiserVersionIds.resize(DenoiserVersionCount);
        m_DenoiserVersionStrings.resize(DenoiserVersionCount);

        queryVersionsDesc.versionIds = m_DenoiserVersionIds.data();
        queryVersionsDesc.versionNames = m_DenoiserVersionStrings.data();
        ffx::Query(queryVersionsDesc);
    }

    m_DenoiserAvailable = !m_DenoiserVersionIds.empty();
    if (!m_DenoiserAvailable)
    {
        m_ViewMode = static_cast<int32_t>(ViewMode::InputDefault);
    }

	ffx::QueryDescDenoiserGetDefaultSettings queryDefaultSettingsDesc = {};
	queryDefaultSettingsDesc.device = GetDevice()->GetImpl()->DX12Device();
    queryDefaultSettingsDesc.defaultSettings = &m_DenoiserSettings;
    ffx::Query(queryDefaultSettingsDesc);


    BuildUI();
    EnableModule(true);
    InitContent();
}

void DenoiserRenderModule::EnableModule(bool enabled)
{
    if (enabled)
    {
        InitDenoiserContext();
        SetModuleEnabled(enabled);
    }
    else
    {
        SetModuleEnabled(enabled);
        DestroyContext();
    }
}

void DenoiserRenderModule::OnPreFrame()
{
    if (NeedsReInit())
    {
        GetDevice()->FlushAllCommandQueues();
        EnableModule(false);
        EnableModule(true);
        ClearReInit();
    }
}

void DenoiserRenderModule::Execute(double deltaTime, cauldron::CommandList* pCmdList)
{
    GPUScopedProfileCapture sampleMarker(pCmdList, L"FSR Ray Regeneration");

    Vec3 dominantLightDir;
    Vec3 dominantLightEmission;
    uint32_t dominantLightIndex = 0;
    const SceneLightingInformation& sceneLightInfo = GetScene()->GetSceneLightInfo();
    if (UseDominantLightVisibility())
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(sceneLightInfo.LightCount); ++i)
        {
            if (sceneLightInfo.LightInfo[i].Type == static_cast<uint32_t>(LightType::Directional))
            {
                dominantLightDir = sceneLightInfo.LightInfo[i].DirectionRange.getXYZ();
                dominantLightEmission = sceneLightInfo.LightInfo[i].ColorIntensity.getXYZ() * sceneLightInfo.LightInfo[i].ColorIntensity.getW();
                dominantLightIndex = i;
                break;
            }
        }
    }

    DispatchPrePass(deltaTime, pCmdList);
    if (m_DenoiserAvailable)
    {
        DispatchDenoiser(deltaTime, pCmdList, dominantLightDir, dominantLightEmission);
    }
    DispatchComposition(deltaTime, pCmdList, sceneLightInfo, dominantLightIndex);
}

void DenoiserRenderModule::OnResize(const cauldron::ResolutionInfo& resInfo)
{
    m_NeedReInit = true;
}

void DenoiserRenderModule::UpdateUI(double /*deltaTime*/)
{
}

void DenoiserRenderModule::BuildUI()
{
    UISection* uiSection = GetUIManager()->RegisterUIElements("FSR Ray Regeneration", UISectionType::Sample);

    if (!m_DenoiserAvailable)
    {
        uiSection->RegisterUIElement<UIText>("Denoiser is not available on this device.");
    }

    uiSection->RegisterUIElement<UICombo>("Version", (int32_t&)m_SelectedDenoiserVersion, m_DenoiserVersionStrings, (bool&)m_DenoiserAvailable, [this](int32_t, int32_t) { m_NeedReInit = true; });

    std::vector<const char*> modes =
    {
        "4 Signals",
        "2 Signals",
        "1 Signal",
    };
    uiSection->RegisterUIElement<UICombo>("Mode", (int32_t&)m_DenoiserMode, std::move(modes), (bool&)m_DenoiserAvailable, [this](int32_t, int32_t) { m_NeedReInit = true; });
    uiSection->RegisterUIElement<UICheckBox>("Denoise dominant light visibility", (bool&)m_EnableDominantLightVisibilityDenoising, (bool&)m_DenoiserAvailable, [this](int32_t cur, int32_t old) { m_NeedReInit = true; });

    uiSection->RegisterUIElement<UICheckBox>("Enable debugging", (bool&)m_EnableDebugging, (bool&)m_DenoiserAvailable, [this](int32_t cur, int32_t old) { m_NeedReInit = true; });

    uiSection->RegisterUIElement<UISlider<float>>("History rejection strength", (float&)m_DenoiserSettings.historyRejectionStrength, 0.0f, 1.0f, (bool&)m_DenoiserAvailable, [this](float cur, float old) { ConfigureSettings(); });
    uiSection->RegisterUIElement<UISlider<float>>("Cross bilateral normal strength", (float&)m_DenoiserSettings.crossBilateralNormalStrength, 0.0f, 1.0f, (bool&)m_DenoiserAvailable, [this](float cur, float old) { ConfigureSettings(); });
    uiSection->RegisterUIElement<UISlider<float>>("Stability bias", (float&)m_DenoiserSettings.stabilityBias, 0.0f, 1.0f, (bool&)m_DenoiserAvailable, [this](float cur, float old) { ConfigureSettings(); });
    uiSection->RegisterUIElement<UISlider<float>>("Max radiance", (float&)m_DenoiserSettings.maxRadiance, 0.0f, 100000.0f, (bool&)m_DenoiserAvailable, [this](float cur, float old) { ConfigureSettings(); });
    uiSection->RegisterUIElement<UISlider<float>>("Radiance Std Clip", (float&)m_DenoiserSettings.radianceClipStdK, 0.0f, 100000.0f, (bool&)m_DenoiserAvailable, [this](float cur, float old) { ConfigureSettings(); });
    uiSection->RegisterUIElement<UISlider<float>>("Gaussian Kernel Relaxation", (float&)m_DenoiserSettings.gaussianKernelRelaxation, 0.0f, 1.0f, (bool&)m_DenoiserAvailable, [this](float cur, float old) { ConfigureSettings(); });

    std::vector<const char*> viewModes =
    {
        "Default",
        "Default (Input)",
        "Direct",
        "Direct diffuse",
        "Direct specular",
        "Indirect",
        "Indirect diffuse",
        "Indirect specular",
        "Direct (Input)",
        "Direct diffuse (Input)",
        "Direct specular (Input)",
        "Indirect (Input)",
        "Indirect diffuse (Input)",
        "Indirect specular (Input)",
        "Linear depth",
        "Motion vectors",
        "Normals",
        "Specular albedo",
        "Diffuse albedo",
        "Fused albedo",
        "Skip signal"
    };
    uiSection->RegisterUIElement<UICombo>("View mode", (int32_t&)m_ViewMode, std::move(viewModes), (bool&)m_DenoiserAvailable);
    uiSection->RegisterUIElement<UICheckBox>("R", (bool&)m_DebugShowChannelR, (bool&)m_DenoiserAvailable, nullptr, true, false);
    uiSection->RegisterUIElement<UICheckBox>("G", (bool&)m_DebugShowChannelG, (bool&)m_DenoiserAvailable, nullptr, true, true);
    uiSection->RegisterUIElement<UICheckBox>("B", (bool&)m_DebugShowChannelB, (bool&)m_DenoiserAvailable, nullptr, true, true);
    uiSection->RegisterUIElement<UICheckBox>("A", (bool&)m_DebugShowChannelA, (bool&)m_DenoiserAvailable, nullptr, true, true);
    uiSection->RegisterUIElement<UIButton>("Reset", (bool&)m_DenoiserAvailable, [this]() { m_ForceReset = true; });
}

static ffxReturnCode_t AllocateResource(uint32_t effectId,
                                        D3D12_RESOURCE_STATES initialState,
                                        const D3D12_HEAP_PROPERTIES* pHeapProps,
                                        const D3D12_RESOURCE_DESC* pD3DDesc,
                                        const FfxApiResourceDescription* pFfxDesc,
                                        const D3D12_CLEAR_VALUE* pOptimizedClear,
                                        ID3D12Resource** ppD3DResource)
{
    ID3D12Device* device = GetDevice()->GetImpl()->DX12Device();
	HRESULT hr = device->CreateCommittedResource(
		pHeapProps,
		D3D12_HEAP_FLAG_NONE,
		pD3DDesc,
		initialState,
		pOptimizedClear,
		IID_PPV_ARGS(ppD3DResource));

    CAUDRON_LOG_INFO(L"Allocated FFX Resource through callback.");
	return SUCCEEDED(hr) ? FFX_API_RETURN_OK : FFX_API_RETURN_ERROR;
}

static ffxReturnCode_t DellocateResource(uint32_t effectId, ID3D12Resource* pResource)
{
    if (pResource)
    {
        CAUDRON_LOG_INFO(L"Deallocated FFX Resource through callback.");
		pResource->Release();
    }
    return FFX_API_RETURN_OK;
}

ffxReturnCode_t AllocateHeap(uint32_t effectId, const D3D12_HEAP_DESC* pHeapDesc, bool aliasable, ID3D12Heap** ppD3DHeap, uint64_t* pHeapStartOffset)
{
    ID3D12Device* device = GetDevice()->GetImpl()->DX12Device();
    HRESULT hr = device->CreateHeap(
        pHeapDesc,
        IID_PPV_ARGS(ppD3DHeap));

    CAUDRON_LOG_INFO(L"Allocated %s FFX heap with size %.3f MB through callback.", aliasable ? L"aliasable":L"persistent", pHeapDesc->SizeInBytes / 1048576.f);
    return SUCCEEDED(hr) ? FFX_API_RETURN_OK : FFX_API_RETURN_ERROR;
}

ffxReturnCode_t DeallocateHeap(uint32_t effectId, ID3D12Heap* pD3DHeap, uint64_t heapStartOffset, uint64_t heapSize)
{
    if (pD3DHeap)
    {
        CAUDRON_LOG_INFO(L"Deallocated FFX Heap through callback.");
		pD3DHeap->Release();
    }
    return FFX_API_RETURN_OK;
}

bool DenoiserRenderModule::InitDenoiserContext()
{
    if (!m_DenoiserAvailable)
        return true;

    const ResolutionInfo& resInfo = GetFramework()->GetResolutionInfo();

    ffx::CreateBackendDX12Desc dx12BackendDesc = {};
    dx12BackendDesc.device = GetDevice()->GetImpl()->DX12Device();

    ffx::CreateBackendDX12AllocationCallbacksDesc dx12BackendAllocatorsDesc = {};
    dx12BackendAllocatorsDesc.pfnFfxResourceAllocator = &AllocateResource;
    dx12BackendAllocatorsDesc.pfnFfxResourceDeallocator = &DellocateResource;
	dx12BackendAllocatorsDesc.pfnFfxHeapAllocator = &AllocateHeap;
	dx12BackendAllocatorsDesc.pfnFfxHeapDeallocator = &DeallocateHeap;
    dx12BackendAllocatorsDesc.pfnFfxConstantBufferAllocator = nullptr;

    ffx::CreateContextDescDenoiser denoiserContextDesc = {};
    denoiserContextDesc.version = FFX_DENOISER_VERSION;
    denoiserContextDesc.maxRenderSize = {resInfo.UpscaleWidth, resInfo.UpscaleHeight};
    denoiserContextDesc.mode = m_DenoiserMode;

    if (m_EnableDebugging)
    {
        denoiserContextDesc.flags |= FFX_DENOISER_ENABLE_DEBUGGING;
    }
    if (m_EnableDominantLightVisibilityDenoising)
    {
        denoiserContextDesc.flags |= FFX_DENOISER_ENABLE_DOMINANT_LIGHT;
    }

    denoiserContextDesc.fpMessage = [](uint32_t type, const wchar_t* message)
        {
            if (type == FFX_API_MESSAGE_TYPE_WARNING)
                cauldron::CauldronWarning(message);
            else
                cauldron::CauldronError(message);
        };

    ffx::CreateContextDescOverrideVersion versionOverride = {};
    versionOverride.versionId = m_DenoiserVersionIds[m_SelectedDenoiserVersion];

    FfxApiEffectMemoryUsage memory = {};
    ffx::QueryDescDenoiserGetGPUMemoryUsage queryMemoryDesc = {};
    queryMemoryDesc.device = dx12BackendDesc.device;
    queryMemoryDesc.maxRenderSize = denoiserContextDesc.maxRenderSize;
    queryMemoryDesc.mode = denoiserContextDesc.mode;
    queryMemoryDesc.flags = denoiserContextDesc.flags;
    queryMemoryDesc.gpuMemoryUsage = &memory;
    ffx::Query(queryMemoryDesc, versionOverride);
    CAUDRON_LOG_INFO(L"Denoiser version %S Query GPUMemoryUsage VRAM totalUsageInBytes %.3f MB aliasableUsageInBytes %.3f MB", m_DenoiserVersionStrings[m_SelectedDenoiserVersion], memory.totalUsageInBytes / 1048576.f, memory.aliasableUsageInBytes / 1048576.f);

    ffx::ReturnCode retCode = ffx::CreateContext(m_pDenoiserContext, nullptr, denoiserContextDesc, dx12BackendDesc, dx12BackendAllocatorsDesc, versionOverride);
    CauldronAssert(ASSERT_CRITICAL, retCode == ffx::ReturnCode::Ok, L"Couldn't create the denoiser context: %d", (uint32_t)retCode);

    uint32_t versionMajor = 0;
    uint32_t versionMinor = 0;
    uint32_t versionPatch = 0;
    ffx::QueryDescDenoiserGetVersion versionQuery = {};
    versionQuery.device = dx12BackendDesc.device;
    versionQuery.major = &versionMajor;
    versionQuery.minor = &versionMinor;
    versionQuery.patch = &versionPatch;
    ffx::Query(m_pDenoiserContext, versionQuery);
    CAUDRON_LOG_INFO(L"Queried denoiser version: %i.%i.%i", versionMajor, versionMinor, versionPatch);

    ConfigureSettings();

    return retCode == ffx::ReturnCode::Ok;
}

void DenoiserRenderModule::DestroyContext()
{
    if (m_pDenoiserContext)
    {
        ffx::DestroyContext(m_pDenoiserContext);
        m_pDenoiserContext = nullptr;
    }
}

bool DenoiserRenderModule::InitResources()
{
    auto renderSizeFn = [](TextureDesc& desc, uint32_t displayWidth, uint32_t displayHeight, uint32_t renderingWidth, uint32_t renderingHeight) {
        desc.Width  = renderingWidth;
        desc.Height = renderingHeight;
    };
    auto displaySizeFn = [](TextureDesc& desc, uint32_t displayWidth, uint32_t displayHeight, uint32_t renderingWidth, uint32_t renderingHeight)
    {
        desc.Width = displayWidth;
        desc.Height = displayHeight;
    };

    m_pColorTarget = GetFramework()->GetColorTargetForCallback(GetName());
    m_pDepthTarget = GetFramework()->GetRenderTexture(L"DepthTarget");
    m_pGBufferMotionVectors = GetFramework()->GetRenderTexture(L"GBufferMotionVectorRT");

    m_pDirectDiffuse = GetFramework()->GetRenderTexture(L"DenoiserDirectDiffuseTarget");
    m_pDirectSpecular = GetFramework()->GetRenderTexture(L"DenoiserDirectSpecularTarget");
    m_pIndirectDiffuse = GetFramework()->GetRenderTexture(L"DenoiserIndirectDiffuseTarget");
    m_pIndirectRayDirDiffuse = GetFramework()->GetRenderTexture(L"DenoiserIndirectDiffuseRayDirTarget");
    m_pIndirectSpecular = GetFramework()->GetRenderTexture(L"DenoiserIndirectSpecularTarget");
    m_pIndirectRayDirSpecular = GetFramework()->GetRenderTexture(L"DenoiserIndirectSpecularRayDirTarget");
    m_pDominantLightVisibility = GetFramework()->GetRenderTexture(L"DenoiserDominantLightVisibilityTarget");

    m_pDiffuseAlbedo = GetFramework()->GetRenderTexture(L"DenoiserDiffuseAlbedoTarget");
    m_pSpecularAlbedo = GetFramework()->GetRenderTexture(L"DenoiserSpecularAlbedoTarget");
    m_pFusedAlbedo = GetFramework()->GetRenderTexture(L"DenoiserFusedAlbedoTarget");
    m_pNormals = GetFramework()->GetRenderTexture(L"DenoiserNormalsTarget");
    m_pSkipSignal = GetFramework()->GetRenderTexture(L"DenoiserSkipSignalTarget");

    cauldron::TextureDesc desc = m_pDirectDiffuse->GetDesc();
    desc.Flags |= ResourceFlags::AllowUnorderedAccess;

    desc.Name = L"Denoiser_DenoisedDirectDiffuse";
    m_pDenoisedDirectDiffuse = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    desc.Name = L"Denoiser_DenoisedDirectSpecular";
    m_pDenoisedDirectSpecular = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    desc.Name = L"Denoiser_DenoisedIndirectDiffuse";
    m_pDenoisedIndirectDiffuse = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    desc.Name = L"Denoiser_DenoisedIndirectSpecular";
    m_pDenoisedIndirectSpecular = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    desc.Name = L"Denoiser_DenoisedDominantLightVisibility";
    desc.Format = ResourceFormat::R16_FLOAT;
    m_pDenoisedDominantLightVisibility = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    desc.Format = ResourceFormat::R32_FLOAT;
    desc.Name = L"Denoiser_LinearDepth";
    m_pLinearDepth = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    desc.Format = ResourceFormat::RGBA16_FLOAT;
    desc.Name = L"Denoiser_MotionVectors";
    m_pMotionVectors = GetDynamicResourcePool()->CreateRenderTexture(&desc, displaySizeFn);

    m_pPrePassParameterSet->SetTextureSRV(m_pDepthTarget, ViewDimension::Texture2D, 0);
    m_pPrePassParameterSet->SetTextureSRV(m_pGBufferMotionVectors, ViewDimension::Texture2D, 1);
    m_pPrePassParameterSet->SetTextureUAV(m_pLinearDepth, ViewDimension::Texture2D, 0);
    m_pPrePassParameterSet->SetTextureUAV(m_pMotionVectors, ViewDimension::Texture2D, 1);
    m_pPrePassParameterSet->SetTextureUAV(m_pDenoisedDirectDiffuse, ViewDimension::Texture2D, 2);
    m_pPrePassParameterSet->SetTextureUAV(m_pDenoisedDirectSpecular, ViewDimension::Texture2D, 3);
    m_pPrePassParameterSet->SetTextureUAV(m_pDenoisedIndirectDiffuse, ViewDimension::Texture2D, 4);
    m_pPrePassParameterSet->SetTextureUAV(m_pDenoisedIndirectSpecular, ViewDimension::Texture2D, 5);
    m_pPrePassParameterSet->SetTextureUAV(m_pDenoisedDominantLightVisibility, ViewDimension::Texture2D, 6);

    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedDirectDiffuse, ViewDimension::Texture2D, 0);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedDirectSpecular, ViewDimension::Texture2D, 1);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedIndirectDiffuse, ViewDimension::Texture2D, 2);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedIndirectSpecular, ViewDimension::Texture2D, 3);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedDominantLightVisibility, ViewDimension::Texture2D, 4);
    m_pComposeParameterSet->SetTextureSRV(m_pSkipSignal, ViewDimension::Texture2D, 5);
    m_pComposeParameterSet->SetTextureSRV(m_pDiffuseAlbedo, ViewDimension::Texture2D, 6);
    m_pComposeParameterSet->SetTextureSRV(m_pSpecularAlbedo, ViewDimension::Texture2D, 7);
    m_pComposeParameterSet->SetTextureSRV(m_pFusedAlbedo, ViewDimension::Texture2D, 8);
    m_pComposeParameterSet->SetTextureSRV(m_pNormals, ViewDimension::Texture2D, 9);
    m_pComposeParameterSet->SetTextureSRV(m_pDepthTarget, ViewDimension::Texture2D, 10);
    m_pComposeParameterSet->SetTextureUAV(m_pColorTarget, ViewDimension::Texture2D, 0);

    return true;
}

bool DenoiserRenderModule::InitPipelineObjects()
{
    RootSignatureDesc prePassRootSignatureDesc;
    prePassRootSignatureDesc.AddConstantBufferView(0, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureSRVSet(0, ShaderBindStage::Compute, 1); // Depth
    prePassRootSignatureDesc.AddTextureSRVSet(1, ShaderBindStage::Compute, 1); // Motion Vectors
    //prePassRootSignatureDesc.AddStaticSamplers(0, ShaderBindStage::Compute, 1, &m_BilinearSampler);
    prePassRootSignatureDesc.AddTextureUAVSet(0, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureUAVSet(1, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureUAVSet(2, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureUAVSet(3, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureUAVSet(4, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureUAVSet(5, ShaderBindStage::Compute, 1);
    prePassRootSignatureDesc.AddTextureUAVSet(6, ShaderBindStage::Compute, 1);
    m_pPrePassRootSignature = RootSignature::CreateRootSignature(L"PrePass_RootSignature", prePassRootSignatureDesc);
    if (!m_pPrePassRootSignature)
        return false;

    PipelineDesc prePassPipelineDesc;
    prePassPipelineDesc.SetRootSignature(m_pPrePassRootSignature);
    ShaderBuildDesc prePassDesc = ShaderBuildDesc::Compute(L"denoiser_prepass.hlsl", L"main", ShaderModel::SM6_6, nullptr);
    prePassPipelineDesc.AddShaderDesc(prePassDesc);
    m_pPrePassPipeline = PipelineObject::CreatePipelineObject(L"PrePass_Pipeline", prePassPipelineDesc);
    if (!m_pPrePassPipeline)
        return false;

    m_pPrePassParameterSet = ParameterSet::CreateParameterSet(m_pPrePassRootSignature);
    m_pPrePassParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(PrePassConstants), 0);

    RootSignatureDesc composeRootSignatureDesc;
    composeRootSignatureDesc.AddConstantBufferView(0, ShaderBindStage::Compute, 1);
    composeRootSignatureDesc.AddConstantBufferView(1, ShaderBindStage::Compute, 1);
    composeRootSignatureDesc.AddTextureSRVSet(0, ShaderBindStage::Compute, 1); // Direct diffuse
    composeRootSignatureDesc.AddTextureSRVSet(1, ShaderBindStage::Compute, 1); // Direct specular
    composeRootSignatureDesc.AddTextureSRVSet(2, ShaderBindStage::Compute, 1); // Indirect diffuse
    composeRootSignatureDesc.AddTextureSRVSet(3, ShaderBindStage::Compute, 1); // Indirect specular
    composeRootSignatureDesc.AddTextureSRVSet(4, ShaderBindStage::Compute, 1); // Dominant light visibility
    composeRootSignatureDesc.AddTextureSRVSet(5, ShaderBindStage::Compute, 1); // Skip signal
    composeRootSignatureDesc.AddTextureSRVSet(6, ShaderBindStage::Compute, 1); // Diffuse albedo
    composeRootSignatureDesc.AddTextureSRVSet(7, ShaderBindStage::Compute, 1); // Specular albedo
    composeRootSignatureDesc.AddTextureSRVSet(8, ShaderBindStage::Compute, 1); // Fused albedo
    composeRootSignatureDesc.AddTextureSRVSet(9, ShaderBindStage::Compute, 1); // Normals
    composeRootSignatureDesc.AddTextureSRVSet(10, ShaderBindStage::Compute, 1); // Depth
    //composeRootSignatureDesc.AddStaticSamplers(0, ShaderBindStage::Compute, 1, &m_BilinearSampler);
    composeRootSignatureDesc.AddTextureUAVSet(0, ShaderBindStage::Compute, 1);
    m_pComposeRootSignature = RootSignature::CreateRootSignature(L"Compose_RootSignature", composeRootSignatureDesc);
    if (!m_pComposeRootSignature)
        return false;

    PipelineDesc composePipelineDesc;
    composePipelineDesc.SetRootSignature(m_pComposeRootSignature);
    ShaderBuildDesc composeDesc = ShaderBuildDesc::Compute(L"denoiser_compose.hlsl", L"main", ShaderModel::SM6_6, nullptr);
    composePipelineDesc.AddShaderDesc(composeDesc);
    m_pComposePipeline = PipelineObject::CreatePipelineObject(L"Compose_Pipeline", composePipelineDesc);
    if (!m_pComposePipeline)
        return false;

    m_pComposeParameterSet = ParameterSet::CreateParameterSet(m_pComposeRootSignature);
    m_pComposeParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(ComposeConstants), 0);
    m_pComposeParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(SceneLightingInformation), 1);

    return true;
}

bool DenoiserRenderModule::InitContent()
{
    // Need to create our content on a background thread so proper notifiers can be called
    std::function<void(void*)> createContent = [this](void*)
        {
            CameraComponentData cameraComponentData = {};
            cameraComponentData.Name = L"DenoiserCamera";
            cameraComponentData.Type = CameraType::Perspective;
            cameraComponentData.Zfar = 1024.0f;
            cameraComponentData.Perspective.AspectRatio = GetFramework()->GetAspectRatio();
            cameraComponentData.Perspective.Yfov = DEG_TO_RAD(80.0f);

            ContentBlock* pContentBlock = new ContentBlock();

            // Memory backing camera creation
            EntityDataBlock* pCameraDataBlock = new EntityDataBlock();
            pContentBlock->EntityDataBlocks.push_back(pCameraDataBlock);
            pCameraDataBlock->pEntity = new Entity(cameraComponentData.Name.c_str());
            m_pDenoiserCamera = pCameraDataBlock->pEntity;
            CauldronAssert(ASSERT_CRITICAL, m_pDenoiserCamera, L"Could not allocate denoiser camera entity");

            // Calculate transform
            Mat4 lookAt = Mat4::lookAt(Point3(-6.8f, 2.0f, -5.8f), Point3(1.0f, 0.5f, -0.5f), Vec3(0.0f, 1.0f, 0.0f));
            Mat4 transform = InverseMatrix(lookAt);
            m_pDenoiserCamera->SetTransform(transform);

            CameraComponentData* pCameraComponentData = new CameraComponentData(cameraComponentData);
            pCameraDataBlock->ComponentsData.push_back(pCameraComponentData);
            m_pDenoiserCameraComponent = CameraComponentMgr::Get()->SpawnCameraComponent(m_pDenoiserCamera, pCameraComponentData);
            pCameraDataBlock->Components.push_back(m_pDenoiserCameraComponent);
            pContentBlock->ActiveCamera = m_pDenoiserCamera;

            GetContentManager()->StartManagingContent(L"DenoiserRenderModule", pContentBlock, false);

            // We are now ready for use
            SetModuleReady(true);
        };

    // Queue a task to create needed content after setup (but before run)
    Task createContentTask(createContent, nullptr);
    GetFramework()->AddContentCreationTask(createContentTask);

    return true;
}

void DenoiserRenderModule::ConfigureSettings()
{
    if (m_pDenoiserContext)
    {
        ffx::ConfigureDescDenoiserSettings settings = {};
        settings.settings = m_DenoiserSettings;
        ffx::Configure(m_pDenoiserContext, settings);
    }
}

void DenoiserRenderModule::DispatchPrePass(double deltaTime, cauldron::CommandList* pCmdList)
{
    const ResolutionInfo&   resInfo = GetFramework()->GetResolutionInfo();
    CameraComponent*        pCamera = GetScene()->GetCurrentCamera();

    PrePassConstants constants = {};
    memcpy(constants.clipToCamera, &pCamera->GetInverseProjection(), sizeof(constants.clipToCamera));
    memcpy(constants.clipToWorld, &pCamera->GetInverseViewProjection(), sizeof(constants.clipToWorld));
    memcpy(constants.prevWorldToCamera, &pCamera->GetPreviousView(), sizeof(constants.prevWorldToCamera));
    constants.renderWidth = static_cast<float>(resInfo.RenderWidth);
    constants.renderHeight = static_cast<float>(resInfo.RenderHeight);
    constants.cameraNear = pCamera->GetNearPlane();
    constants.cameraFar = pCamera->GetFarPlane();

    BufferAddressInfo constantsBufferInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(constants), reinterpret_cast<const void*>(&constants));
    m_pPrePassParameterSet->UpdateRootConstantBuffer(&constantsBufferInfo, 0);

    m_pPrePassParameterSet->Bind(pCmdList, m_pPrePassPipeline);
    SetPipelineState(pCmdList, m_pPrePassPipeline);
    const uint32_t numGroupsX = (resInfo.RenderWidth + 7) / 8;
    const uint32_t numGroupsY = (resInfo.RenderHeight + 7) / 8;
    Dispatch(pCmdList, numGroupsX, numGroupsY, 1);
}

void DenoiserRenderModule::DispatchDenoiser(double deltaTime, cauldron::CommandList* pCmdList, const Vec3& dominantLightDir, const Vec3& dominantLightEmission)
{
    if (NeedsReInit())
        return;

    bool                  reset   = m_ForceReset;
    const ResolutionInfo& resInfo = GetFramework()->GetResolutionInfo();
    CameraComponent*      pCamera = GetScene()->GetCurrentCamera();

    ffx::DispatchDescDenoiser dispatchDenoiser = {};
    dispatchDenoiser.commandList          = pCmdList->GetImpl()->DX12CmdList();
    dispatchDenoiser.renderSize.width     = resInfo.RenderWidth;
    dispatchDenoiser.renderSize.height    = resInfo.RenderHeight;
    dispatchDenoiser.motionVectorScale.x  = 1.0f;
    dispatchDenoiser.motionVectorScale.y  = 1.0f;

    const Vec2& jitterOffsets  = pCamera->GetJitterOffsets();
    const Vec3 cameraPosition = pCamera->GetCameraPos();
    const Vec3 cameraPositionDelta = m_PrevCameraPosition - cameraPosition;
    const Vec3 cameraRight    = pCamera->GetCameraRight();
    const Vec3 cameraUp       = pCamera->GetCameraUp();
    const Vec3 cameraForward  = pCamera->GetDirection().getXYZ();

    dispatchDenoiser.jitterOffsets          = {jitterOffsets.getX(), jitterOffsets.getY()};
    dispatchDenoiser.cameraPositionDelta    = {cameraPositionDelta.getX(), cameraPositionDelta.getY(), cameraPositionDelta.getZ()};
    dispatchDenoiser.cameraRight            = {cameraRight.getX(), cameraRight.getY(), cameraRight.getZ()};
    dispatchDenoiser.cameraUp               = {cameraUp.getX(), cameraUp.getY(), cameraUp.getZ()};
    dispatchDenoiser.cameraForward          = {cameraForward.getX(), cameraForward.getY(), cameraForward.getZ()};
    dispatchDenoiser.cameraAspectRatio      = GetFramework()->GetAspectRatio();
    dispatchDenoiser.cameraNear             = pCamera->GetNearPlane();
    dispatchDenoiser.cameraFar              = pCamera->GetFarPlane();
    dispatchDenoiser.cameraFovAngleVertical = pCamera->GetFovY();
    dispatchDenoiser.deltaTime              = static_cast<float>(deltaTime);
    dispatchDenoiser.frameIndex             = static_cast<uint32_t>(GetFramework()->GetFrameID());

    dispatchDenoiser.flags = 0;
    if (reset)
        dispatchDenoiser.flags |= FFX_DENOISER_DISPATCH_RESET;

    dispatchDenoiser.linearDepth = SDKWrapper::ffxGetResourceApi(m_pLinearDepth->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
    dispatchDenoiser.motionVectors = SDKWrapper::ffxGetResourceApi(m_pMotionVectors->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
    dispatchDenoiser.normals = SDKWrapper::ffxGetResourceApi(m_pNormals->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
    dispatchDenoiser.specularAlbedo = SDKWrapper::ffxGetResourceApi(m_pSpecularAlbedo->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
    dispatchDenoiser.diffuseAlbedo = SDKWrapper::ffxGetResourceApi(m_pDiffuseAlbedo->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);

    ffx::DispatchDescDenoiserInputDominantLight dispatchDenoiserInputsDominantLight = {};
    if (m_EnableDominantLightVisibilityDenoising)
    {
        dispatchDenoiserInputsDominantLight.dominantLightVisibility.input = SDKWrapper::ffxGetResourceApi(m_pDominantLightVisibility->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputsDominantLight.dominantLightVisibility.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedDominantLightVisibility->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
        dispatchDenoiserInputsDominantLight.dominantLightEmission = { dominantLightEmission.getX(), dominantLightEmission.getY(), dominantLightEmission.getZ() };
        dispatchDenoiserInputsDominantLight.dominantLightDirection = { -dominantLightDir.getX(), -dominantLightDir.getY(), -dominantLightDir.getZ() };
    }

    if (m_DenoiserMode == FFX_DENOISER_MODE_1_SIGNAL)
    {
        ffx::DispatchDescDenoiserInput1Signal dispatchDenoiserInputs = {};
        dispatchDenoiserInputs.radiance.input = SDKWrapper::ffxGetResourceApi(m_pIndirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.radiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedIndirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
        dispatchDenoiserInputs.fusedAlbedo = SDKWrapper::ffxGetResourceApi(m_pFusedAlbedo->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);

        if (m_EnableDominantLightVisibilityDenoising)
        {
            ffx::Dispatch(m_pDenoiserContext, dispatchDenoiser, dispatchDenoiserInputs, dispatchDenoiserInputsDominantLight);
        }
        else
        {
            ffx::Dispatch(m_pDenoiserContext, dispatchDenoiser, dispatchDenoiserInputs);
        }
    }
    else if (m_DenoiserMode == FFX_DENOISER_MODE_2_SIGNALS)
    {
        ffx::DispatchDescDenoiserInput2Signals dispatchDenoiserInputs = {};
        dispatchDenoiserInputs.specularRadiance.input = SDKWrapper::ffxGetResourceApi(m_pIndirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.specularRadiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedIndirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

        dispatchDenoiserInputs.diffuseRadiance.input = SDKWrapper::ffxGetResourceApi(m_pIndirectDiffuse->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.diffuseRadiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedIndirectDiffuse->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

        if (m_EnableDominantLightVisibilityDenoising)
        {
            ffx::Dispatch(m_pDenoiserContext, dispatchDenoiser, dispatchDenoiserInputs, dispatchDenoiserInputsDominantLight);
        }
        else
        {
            ffx::Dispatch(m_pDenoiserContext, dispatchDenoiser, dispatchDenoiserInputs);
        }
    }
    else //if (m_DenoiserMode == FFX_DENOISER_MODE_4_SIGNALS)
    {
        ffx::DispatchDescDenoiserInput4Signals dispatchDenoiserInputs = {};
        dispatchDenoiserInputs.indirectSpecularRadiance.input = SDKWrapper::ffxGetResourceApi(m_pIndirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.indirectSpecularRadiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedIndirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

        dispatchDenoiserInputs.indirectDiffuseRadiance.input = SDKWrapper::ffxGetResourceApi(m_pIndirectDiffuse->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.indirectDiffuseRadiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedIndirectDiffuse->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

        dispatchDenoiserInputs.directSpecularRadiance.input = SDKWrapper::ffxGetResourceApi(m_pDirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.directSpecularRadiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedDirectSpecular->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

        dispatchDenoiserInputs.directDiffuseRadiance.input = SDKWrapper::ffxGetResourceApi(m_pDirectDiffuse->GetResource(), FFX_API_RESOURCE_STATE_PIXEL_COMPUTE_READ);
        dispatchDenoiserInputs.directDiffuseRadiance.output = SDKWrapper::ffxGetResourceApi(m_pDenoisedDirectDiffuse->GetResource(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

        if (m_EnableDominantLightVisibilityDenoising)
        {
            ffx::Dispatch(m_pDenoiserContext, dispatchDenoiser, dispatchDenoiserInputs, dispatchDenoiserInputsDominantLight);
        }
        else
        {
            ffx::ReturnCode ret = ffx::Dispatch(m_pDenoiserContext, dispatchDenoiser, dispatchDenoiserInputs);
            CAULDRON_ASSERT(ret == ffx::ReturnCode::Ok);
        }
    }

    // Reset all descriptor heaps
    SetAllResourceViewHeaps(pCmdList);
    m_ForceReset = false;
    m_PrevCameraPosition = cameraPosition;
}

void DenoiserRenderModule::DispatchComposition(double deltaTime, cauldron::CommandList* pCmdList, const SceneLightingInformation& sceneLightInfo, uint32_t dominantLightIndex)
{
    const ResolutionInfo&   resInfo = GetFramework()->GetResolutionInfo();
    CameraComponent*        pCamera = GetScene()->GetCurrentCamera();

    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedDirectDiffuse, ViewDimension::Texture2D, 0);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedDirectSpecular, ViewDimension::Texture2D, 1);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedIndirectDiffuse, ViewDimension::Texture2D, 2);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedIndirectSpecular, ViewDimension::Texture2D, 3);
    m_pComposeParameterSet->SetTextureSRV(m_pDenoisedDominantLightVisibility, ViewDimension::Texture2D, 4);

    ComposeConstants constants = {};
    constants.channelContrib = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    constants.flags = 0;

    if (m_DenoiserMode == FFX_DENOISER_MODE_1_SIGNAL)
        constants.flags |= COMPOSE_FUSED;

    const ViewMode viewMode  = static_cast<ViewMode>(m_ViewMode);
    switch (viewMode)
    {
    default:
    case ViewMode::Default:
        constants.directDiffuseContrib = 1.0f;
        constants.directSpecularContrib = 1.0f;
        constants.indirectDiffuseContrib = 1.0f;
        constants.indirectSpecularContrib = 1.0f;
        constants.skipContrib = 1.0f;
        break;
    case ViewMode::InputDefault:
        constants.directDiffuseContrib    = 1.0f;
        constants.directSpecularContrib   = 1.0f;
        constants.indirectDiffuseContrib  = 1.0f;
        constants.indirectSpecularContrib = 1.0f;
        constants.skipContrib             = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pDirectDiffuse, ViewDimension::Texture2D, 0);
        m_pComposeParameterSet->SetTextureSRV(m_pDirectSpecular, ViewDimension::Texture2D, 1);
        m_pComposeParameterSet->SetTextureSRV(m_pIndirectDiffuse, ViewDimension::Texture2D, 2);
        m_pComposeParameterSet->SetTextureSRV(m_pIndirectSpecular, ViewDimension::Texture2D, 3);
        m_pComposeParameterSet->SetTextureSRV(m_pDominantLightVisibility, ViewDimension::Texture2D, 4);
        break;
    case ViewMode::Direct:
        constants.directDiffuseContrib = 1.0f;
        constants.directSpecularContrib = 1.0f;
        break;
    case ViewMode::DirectDiffuse:
        constants.directDiffuseContrib = 1.0f;
        break;
    case ViewMode::DirectSpecular:
        constants.directSpecularContrib = 1.0f;
        break;
    case ViewMode::Indirect:
        constants.indirectDiffuseContrib = 1.0f;
        constants.indirectSpecularContrib = 1.0f;
        break;
    case ViewMode::IndirectDiffuse:
        constants.indirectDiffuseContrib = 1.0f;
        break;
    case ViewMode::IndirectSpecular:
        constants.indirectSpecularContrib = 1.0f;
        break;
    case ViewMode::InputDirect:
        constants.directDiffuseContrib = 1.0f;
        constants.directSpecularContrib = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pDirectDiffuse, ViewDimension::Texture2D, 0);
        m_pComposeParameterSet->SetTextureSRV(m_pDirectSpecular, ViewDimension::Texture2D, 1);
        m_pComposeParameterSet->SetTextureSRV(m_pDominantLightVisibility, ViewDimension::Texture2D, 4);
        break;
    case ViewMode::InputDirectDiffuse:
        constants.directDiffuseContrib = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pDirectDiffuse, ViewDimension::Texture2D, 0);
        m_pComposeParameterSet->SetTextureSRV(m_pDominantLightVisibility, ViewDimension::Texture2D, 4);
        break;
    case ViewMode::InputDirectSpecular:
        constants.directSpecularContrib = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pDirectSpecular, ViewDimension::Texture2D, 1);
        m_pComposeParameterSet->SetTextureSRV(m_pDominantLightVisibility, ViewDimension::Texture2D, 4);
        break;
    case ViewMode::InputIndirect:
        constants.indirectDiffuseContrib  = 1.0f;
        constants.indirectSpecularContrib = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pIndirectDiffuse, ViewDimension::Texture2D, 2);
        m_pComposeParameterSet->SetTextureSRV(m_pIndirectSpecular, ViewDimension::Texture2D, 3);
        break;
    case ViewMode::InputIndirectDiffuse:
        constants.indirectDiffuseContrib = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pIndirectDiffuse, ViewDimension::Texture2D, 2);
        break;
    case ViewMode::InputIndirectSpecular:
        constants.indirectSpecularContrib = 1.0f;
        m_pComposeParameterSet->SetTextureSRV(m_pIndirectSpecular, ViewDimension::Texture2D, 3);
        break;
    case ViewMode::InputLinearDepth:
        constants.directDiffuseContrib = 1.0f;
        constants.flags |= COMPOSE_DEBUG_MODE;
        constants.flags |= COMPOSE_DEBUG_USE_RANGE;
        constants.flags |= COMPOSE_DEBUG_ABS_VALUE;
        constants.flags |= COMPOSE_DEBUG_ONLY_FIRST_RESOURCE;
        constants.rangeMin = 0.0f;
        constants.rangeMax = 100.0f;
        constants.channelContrib = Vec4((float)m_DebugShowChannelR, (float)m_DebugShowChannelG, (float)m_DebugShowChannelB, (float)m_DebugShowChannelA);
        m_pComposeParameterSet->SetTextureSRV(m_pLinearDepth, ViewDimension::Texture2D, 0);
        break;
    case ViewMode::InputMotionVectors:
        constants.directDiffuseContrib = 1.0f;
        constants.flags |= COMPOSE_DEBUG_MODE;
        constants.flags |= COMPOSE_DEBUG_ONLY_FIRST_RESOURCE;
        constants.channelContrib = Vec4((float)m_DebugShowChannelR, (float)m_DebugShowChannelG, (float)m_DebugShowChannelB, (float)m_DebugShowChannelA);
        m_pComposeParameterSet->SetTextureSRV(m_pMotionVectors, ViewDimension::Texture2D, 0);
        break;
    case ViewMode::InputNormals:
        constants.directDiffuseContrib = 1.0f;
        constants.flags |= COMPOSE_DEBUG_MODE;
        constants.flags |= COMPOSE_DEBUG_DECODE_NORMALS;
        constants.flags |= COMPOSE_DEBUG_ONLY_FIRST_RESOURCE;
        constants.channelContrib = Vec4((float)m_DebugShowChannelR, (float)m_DebugShowChannelG, (float)m_DebugShowChannelB, (float)m_DebugShowChannelA);
        m_pComposeParameterSet->SetTextureSRV(m_pNormals, ViewDimension::Texture2D, 0);
        break;
    case ViewMode::InputSpecularAlbedo:
        constants.directDiffuseContrib = 1.0f;
        constants.flags |= COMPOSE_DEBUG_MODE;
        constants.flags |= COMPOSE_DEBUG_DECODE_SQRT;
        constants.flags |= COMPOSE_DEBUG_ONLY_FIRST_RESOURCE;
        constants.channelContrib = Vec4((float)m_DebugShowChannelR, (float)m_DebugShowChannelG, (float)m_DebugShowChannelB, (float)m_DebugShowChannelA);
        m_pComposeParameterSet->SetTextureSRV(m_pSpecularAlbedo, ViewDimension::Texture2D, 0);
        break;
    case ViewMode::InputDiffuseAlbedo:
        constants.directDiffuseContrib = 1.0f;
        constants.flags |= COMPOSE_DEBUG_MODE;
        constants.flags |= COMPOSE_DEBUG_DECODE_SQRT;
        constants.flags |= COMPOSE_DEBUG_ONLY_FIRST_RESOURCE;
        constants.channelContrib = Vec4((float)m_DebugShowChannelR, (float)m_DebugShowChannelG, (float)m_DebugShowChannelB, (float)m_DebugShowChannelA);
        m_pComposeParameterSet->SetTextureSRV(m_pDiffuseAlbedo, ViewDimension::Texture2D, 0);
        break;
    case ViewMode::InputFusedAlbedo:
        constants.directDiffuseContrib = 1.0f;
        constants.flags |= COMPOSE_DEBUG_MODE;
        constants.flags |= COMPOSE_DEBUG_DECODE_SQRT;
        constants.flags |= COMPOSE_DEBUG_ONLY_FIRST_RESOURCE;
        constants.channelContrib = Vec4((float)m_DebugShowChannelR, (float)m_DebugShowChannelG, (float)m_DebugShowChannelB, (float)m_DebugShowChannelA);
        m_pComposeParameterSet->SetTextureSRV(m_pFusedAlbedo, ViewDimension::Texture2D, 0);
        break;
    case ViewMode::InputSkipSignal:
        constants.skipContrib = 1.0f;
        break;
    }

    constants.useDominantLight = static_cast<uint32_t>(UseDominantLightVisibility());
    constants.dominantLightIndex = dominantLightIndex;

    memcpy(&constants.clipToWorld, &pCamera->GetInverseViewProjection(), sizeof(constants.clipToWorld));
    memcpy(&constants.cameraToWorld, &pCamera->GetInverseView(), sizeof(constants.cameraToWorld));
    constants.invRenderSize[0] = 1.0f / resInfo.RenderWidth;
    constants.invRenderSize[1] = 1.0f / resInfo.RenderHeight;

    BufferAddressInfo constantsBufferInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(constants), reinterpret_cast<const void*>(&constants));
    m_pComposeParameterSet->UpdateRootConstantBuffer(&constantsBufferInfo, 0);

    BufferAddressInfo lightingBufferInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(sceneLightInfo), reinterpret_cast<const void*>(&sceneLightInfo));
    m_pComposeParameterSet->UpdateRootConstantBuffer(&lightingBufferInfo, 1);

    m_pComposeParameterSet->Bind(pCmdList, m_pComposePipeline);
    SetPipelineState(pCmdList, m_pComposePipeline);
    const uint32_t numGroupsX = (resInfo.RenderWidth + 7) / 8;
    const uint32_t numGroupsY = (resInfo.RenderHeight + 7) / 8;
    Dispatch(pCmdList, numGroupsX, numGroupsY, 1);
}
