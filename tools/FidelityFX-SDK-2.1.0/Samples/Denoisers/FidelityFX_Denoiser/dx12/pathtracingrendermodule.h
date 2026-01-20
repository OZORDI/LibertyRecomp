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

#include "denoiserrendermodule.h"

#include <cauldron2/dx12/framework/render/rendermodule.h>
#include <cauldron2/dx12/framework/core/framework.h>
#include "cauldron2/dx12/framework/core/contentmanager.h"
#include <cauldron2/dx12/framework/core/uimanager.h>

#include "shaders/shared.h"

#include <vector>

namespace cauldron
{
    class Texture;
    class Material;
    struct SamplerDesc;
    class ParameterSet;
    class ResourceView;
    class RootSignature;
    class UIRenderModule;
}  // namespace cauldron

class PathTracingRenderModule final : public cauldron::RenderModule, public cauldron::ContentListener
{
public:
    PathTracingRenderModule()
        : RenderModule(L"PathTracingRenderModule")
    {
    }
    ~PathTracingRenderModule() override;

    void Init(const json& initData);
    void EnableModule(bool enabled) override;

    /**
     * @brief   Setup parameters that the Denoiser context needs this frame and then call the FFX Dispatch.
     */
    void Execute(double deltaTime, cauldron::CommandList* pCmdList) override;

    /**
     * @brief   Build UI.
     */
    void BuildUI();

    /**
     * Prepare shading information for raytracing passes.
     */
    void OnNewContentLoaded(cauldron::ContentBlock* pContentBlock) override;
    /**
     * @copydoc ContentListener::OnContentUnloaded()
     */
    void OnContentUnloaded(cauldron::ContentBlock* pContentBlock) override;

private:
    bool InitPipelineObjects();
    bool InitResources();
    int32_t AddTexture(const cauldron::Material* pMaterial, const cauldron::TextureClass textureClass, int32_t& textureSamplerIndex);
    void RemoveTexture(int32_t index);

private:
    struct RTInfoTables
    {
        struct BoundTexture
        {
            const cauldron::Texture* pTexture = nullptr;
            uint32_t count    = 1;
        };

        std::vector<const cauldron::Buffer*> m_VertexBuffers;
        std::vector<const cauldron::Buffer*> m_IndexBuffers;
        std::vector<BoundTexture> m_Textures;
        std::vector<cauldron::Sampler*> m_Samplers;

        std::vector<PTMaterialInfo> m_cpuMaterialBuffer;
        std::vector<PTInstanceInfo> m_cpuInstanceBuffer;
        std::vector<Mat4> m_cpuInstanceTransformBuffer;
        std::vector<PTSurfaceInfo> m_cpuSurfaceBuffer;
        std::vector<uint32_t> m_cpuSurfaceIDsBuffer;

        const cauldron::Buffer* m_pMaterialBuffer = NULL;  // material_id -> Material buffer
        const cauldron::Buffer* m_pSurfaceBuffer = NULL;  // surface_id -> Surface_Info buffer
        const cauldron::Buffer* m_pSurfaceIDsBuffer = NULL;  // flat array of uint32_t
        const cauldron::Buffer* m_pInstanceBuffer = NULL;  // instance_id -> Instance_Info buffer
    };
    RTInfoTables m_rtInfoTables;

    const cauldron::Texture* m_pColorTarget = nullptr;
    const cauldron::Texture* m_pDepthTarget = nullptr;

    cauldron::RootSignature* m_pTraceRaysDenoiserRootSignature = nullptr;
    cauldron::PipelineObject* m_pTraceRaysDenoiserPipeline = nullptr;
    cauldron::ParameterSet*m_pTraceRaysDenoiserParameterSet = nullptr;

    const cauldron::Texture* m_pDirectSpecularOutput = nullptr;
    const cauldron::Texture* m_pDirectDiffuseOutput = nullptr;
    const cauldron::Texture* m_pIndirectSpecularOutput = nullptr;
    const cauldron::Texture* m_pIndirectSpecularRayDirOutput = nullptr;
    const cauldron::Texture* m_pIndirectDiffuseOutput = nullptr;
    const cauldron::Texture* m_pIndirectDiffuseRayDirOutput = nullptr;
    const cauldron::Texture* m_pDominantLightVisibilityOutput = nullptr;
    const cauldron::Texture* m_pNormals = nullptr;
    const cauldron::Texture* m_pSpecularAlbedo = nullptr;
    const cauldron::Texture* m_pFusedAlbedo = nullptr;
    const cauldron::Texture* m_pDiffuseAlbedo = nullptr;
    const cauldron::Texture* m_pSkipSignal = nullptr;

    const cauldron::Texture* m_pPrefilteredEnvironmentMap = nullptr;
    const cauldron::Texture* m_pIrradianceEnvironmentMap = nullptr;
    const cauldron::Texture* m_pBRDFTexture = nullptr;

    cauldron::SamplerDesc m_ComparisonSampler;
    cauldron::SamplerDesc m_SpecularSampler;
    cauldron::SamplerDesc m_DiffuseSampler;

    std::mutex m_Mutex;

    DenoiserRenderModule* m_pDenoiserRenderModule = nullptr;
};
