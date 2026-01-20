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

#pragma once

#include <cauldron2/dx12/framework/render/rendermodule.h>
#include <cauldron2/dx12/framework/core/framework.h>
#include <cauldron2/dx12/framework/core/uimanager.h>

#include <FidelityFX/api/include/ffx_api.hpp>

#include <FidelityFX/denoisers/include/ffx_denoiser.hpp>

namespace cauldron
{
    class Texture;
    class ParameterSet;
    class ResourceView;
    class RootSignature;
    class UIRenderModule;
    class CameraComponent;
    class Entity;
}

class DenoiserRenderModule final : public cauldron::RenderModule
{
public:
    DenoiserRenderModule()
        : RenderModule(L"DenoiserRenderModule")
    {
    }
    ~DenoiserRenderModule() override;

    void Init(const json& initData);
    void EnableModule(bool enabled) override;
    void OnPreFrame() override;

    /**
     * @brief   Setup parameters that the Denoiser context needs this frame and then call the FFX Dispatch.
     */
    void Execute(double deltaTime, cauldron::CommandList* pCmdList) override;

    /**
     * @brief   Recreate the Denoiser context to resize internal resources. Called by the framework when the resolution changes.
     */
    void OnResize(const cauldron::ResolutionInfo& resInfo) override;

    /**
     * Update the Debug Option UI element.
     */
    void UpdateUI(double deltaTime) override;

    /**
     * @brief   Build UI.
     */
    void BuildUI();

    /**
     * @brief   Returns whether or not Denoiser requires sample-side re-initialization.
     */
    bool NeedsReInit() const
    {
        return m_NeedReInit;
    }

    /**
     * @brief   Clears Denoiser re-initialization flag.
     */
    void ClearReInit()
    {
        m_NeedReInit = false;
    }

    bool UseDominantLightVisibility() const { return m_EnableDominantLightVisibilityDenoising; }
    uint32_t GetFuseMode() const { return m_DenoiserMode; }

private:
    bool InitDenoiserContext();
    void DestroyContext();
    bool InitResources();
    bool InitPipelineObjects();
    bool InitContent();

    void ConfigureSettings();

    void DispatchPrePass(double deltaTime, cauldron::CommandList* pCmdList);
    void DispatchDenoiser(double deltaTime, cauldron::CommandList* pCmdList, const Vec3& dominantLightDir, const Vec3& dominantLightEmission);
    void DispatchComposition(double deltaTime, cauldron::CommandList* pCmdList, const SceneLightingInformation& sceneLightInfo, uint32_t dominantLightIndex);

    // Settings
    bool m_DenoiserAvailable = false;
    bool m_EnableDebugging = false;
    FfxApiDenoiserSettings m_DenoiserSettings = {};

    bool m_DebugShowChannelR = true;
    bool m_DebugShowChannelG = true;
    bool m_DebugShowChannelB = true;
    bool m_DebugShowChannelA = true;

    FfxApiDenoiserMode m_DenoiserMode = FFX_DENOISER_MODE_4_SIGNALS;
    bool m_EnableDominantLightVisibilityDenoising = true;

    enum class ViewMode
    {
        Default,
        InputDefault,
        Direct,
        DirectDiffuse,
        DirectSpecular,
        Indirect,
        IndirectDiffuse,
        IndirectSpecular,
        InputDirect,
        InputDirectDiffuse,
        InputDirectSpecular,
        InputIndirect,
        InputIndirectDiffuse,
        InputIndirectSpecular,
        InputLinearDepth,
        InputMotionVectors,
        InputNormals,
        InputSpecularAlbedo,
        InputDiffuseAlbedo,
        InputFusedAlbedo,
        InputSkipSignal,
    };
    int32_t m_ViewMode = 0;

    cauldron::Entity* m_pDenoiserCamera = nullptr;
    cauldron::CameraComponent* m_pDenoiserCameraComponent = nullptr;

    const cauldron::Texture* m_pColorTarget = nullptr;
    const cauldron::Texture* m_pDepthTarget = nullptr;
    const cauldron::Texture* m_pGBufferMotionVectors = nullptr;

    const cauldron::Texture* m_pDirectSpecular = nullptr;
    const cauldron::Texture* m_pDirectDiffuse = nullptr;
    const cauldron::Texture* m_pIndirectSpecular = nullptr;
    const cauldron::Texture* m_pIndirectRayDirSpecular = nullptr;
    const cauldron::Texture* m_pIndirectDiffuse  = nullptr;
    const cauldron::Texture* m_pIndirectRayDirDiffuse  = nullptr;
    const cauldron::Texture* m_pDominantLightVisibility  = nullptr;

    const cauldron::Texture* m_pLinearDepth = nullptr;
    const cauldron::Texture* m_pMotionVectors = nullptr;
    const cauldron::Texture* m_pNormals = nullptr;
    const cauldron::Texture* m_pSpecularAlbedo = nullptr;
    const cauldron::Texture* m_pDiffuseAlbedo = nullptr;
    const cauldron::Texture* m_pFusedAlbedo = nullptr;
    const cauldron::Texture* m_pSkipSignal = nullptr;

    const cauldron::Texture* m_pDenoisedDirectSpecular = nullptr;
    const cauldron::Texture* m_pDenoisedDirectDiffuse = nullptr;
    const cauldron::Texture* m_pDenoisedIndirectSpecular = nullptr;
    const cauldron::Texture* m_pDenoisedIndirectDiffuse = nullptr;
    const cauldron::Texture* m_pDenoisedDominantLightVisibility = nullptr;

    cauldron::SamplerDesc m_BilinearSampler;

    cauldron::RootSignature* m_pPrePassRootSignature = nullptr;
    cauldron::PipelineObject* m_pPrePassPipeline = nullptr;
    cauldron::ParameterSet* m_pPrePassParameterSet = nullptr;
    struct PrePassConstants
    {
        float clipToCamera[16];
        float clipToWorld[16];
        float prevWorldToCamera[16];
        float renderWidth;
        float renderHeight;
        float cameraNear;
        float cameraFar;
    };

    cauldron::RootSignature* m_pComposeRootSignature = nullptr;
    cauldron::PipelineObject* m_pComposePipeline = nullptr;
    cauldron::ParameterSet* m_pComposeParameterSet = nullptr;
    struct ComposeConstants
    {
        Mat4 clipToWorld;
        Mat4 cameraToWorld;

        float directDiffuseContrib;
        float directSpecularContrib;
        float indirectDiffuseContrib;
        float indirectSpecularContrib;

        float skipContrib;
        float rangeMin;
        float rangeMax;
        uint32_t flags;

        Vec4 channelContrib;

        Vec2 invRenderSize;
        uint32_t useDominantLight;
        uint32_t dominantLightIndex;
    };
    enum ComposeFlags
    {
        COMPOSE_DEBUG_MODE = 0x1,
        COMPOSE_DEBUG_USE_RANGE = 0x2,
        COMPOSE_DEBUG_DECODE_SQRT = 0x4,
        COMPOSE_DEBUG_ABS_VALUE = 0x8,
        COMPOSE_DEBUG_DECODE_NORMALS = 0x10,
        COMPOSE_DEBUG_ONLY_FIRST_RESOURCE = 0x20,
        COMPOSE_FUSED = 0x40,
    };

    ffx::Context m_pDenoiserContext = nullptr;

    std::vector<uint64_t> m_DenoiserVersionIds;
    std::vector<const char*> m_DenoiserVersionStrings;
    uint32_t m_SelectedDenoiserVersion = 0;

    Vec3 m_PrevCameraPosition = Vec3(0);

    bool m_NeedReInit = false;
    bool m_ForceReset = false;
};
