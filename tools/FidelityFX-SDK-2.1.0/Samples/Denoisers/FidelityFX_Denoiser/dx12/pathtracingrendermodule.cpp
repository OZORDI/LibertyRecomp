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

#include "pathtracingrendermodule.h"

#include "Cauldron2/dx12/framework/core/scene.h"
#include "Cauldron2/dx12/framework/core/components/meshcomponent.h"
#include "Cauldron2/dx12/framework/render/pipelineobject.h"
#include "Cauldron2/dx12/framework/render/parameterset.h"
#include "Cauldron2/dx12/framework/render/dynamicresourcepool.h"
#include "Cauldron2/dx12/framework/render/dynamicbufferpool.h"
#include "Cauldron2/dx12/framework/render/profiler.h"
#include "Cauldron2/dx12/framework/render/material.h"
#include "Cauldron2/dx12/framework/shaders/lightingcommon.h"


using namespace cauldron;

PathTracingRenderModule::~PathTracingRenderModule()
{
}

void PathTracingRenderModule::Init(const json& initData)
{
    CauldronAssert(ASSERT_CRITICAL, GetFramework()->GetConfig()->RT_1_1, L"Error: Pathtracing requires RT1.1");
    CauldronAssert(ASSERT_CRITICAL, GetFramework()->GetConfig()->MinShaderModel >= ShaderModel::SM6_6, L"Error: Pathtracing requires SM6_6 or greater");

    m_pDenoiserRenderModule = static_cast<DenoiserRenderModule*>(GetFramework()->GetRenderModule("DenoiserRenderModule"));
    CauldronAssert(ASSERT_CRITICAL, m_pDenoiserRenderModule != nullptr, L"Error: DenoiserRenderModule required.");

    m_ComparisonSampler.Comparison = GetConfig()->InvertedDepth ? ComparisonFunc::GreaterEqual : ComparisonFunc::LessEqual;
    m_ComparisonSampler.Filter = FilterFunc::ComparisonMinMagLinearMipPoint;
    m_ComparisonSampler.MaxAnisotropy = 1;

    m_SpecularSampler.AddressW = AddressMode::Wrap;
    m_SpecularSampler.Filter = FilterFunc::MinMagMipLinear;
    m_SpecularSampler.MaxAnisotropy = 1;

    m_DiffuseSampler.Filter = FilterFunc::MinMagMipPoint;
    m_DiffuseSampler.AddressW = AddressMode::Wrap;
    m_DiffuseSampler.MaxAnisotropy = 1;

    if (!InitPipelineObjects())
    {
        CauldronError(L"FidelityFX Denoiser Sample: Error: Could not initialize pathtracing pipeline objects.");
        return;
    }

    if (!InitResources())
    {
        CauldronError(L"FidelityFX Denoiser Sample: Error: Could not initialize pathtracing resources.");
        return;
    }

    BuildUI();

    // Register for content change updates
    GetContentManager()->AddContentListener(this);
    SetModuleReady(true);
}

void PathTracingRenderModule::EnableModule(bool enabled)
{
    if (m_pDenoiserRenderModule && !enabled)
        m_pDenoiserRenderModule->EnableModule(enabled); // Denoiser requires PT module

    cauldron::RenderModule::EnableModule(enabled);
}

void PathTracingRenderModule::Execute(double deltaTime, cauldron::CommandList* pCmdList)
{
    if (!m_pBRDFTexture || !m_pPrefilteredEnvironmentMap || !m_pIrradianceEnvironmentMap)
    {
        m_pBRDFTexture               = GetScene()->GetBRDFLutTexture();
        m_pPrefilteredEnvironmentMap = GetScene()->GetIBLTexture(IBLTexture::Prefiltered);
        m_pIrradianceEnvironmentMap  = GetScene()->GetIBLTexture(IBLTexture::Irradiance);

        // These might not yet be loaded
        if (m_pBRDFTexture && m_pPrefilteredEnvironmentMap && m_pIrradianceEnvironmentMap)
        {
            m_pTraceRaysDenoiserParameterSet->SetTextureSRV(m_pBRDFTexture, ViewDimension::Texture2D, 2);
            m_pTraceRaysDenoiserParameterSet->SetTextureSRV(m_pPrefilteredEnvironmentMap, ViewDimension::TextureCube, 3);
            m_pTraceRaysDenoiserParameterSet->SetTextureSRV(m_pIrradianceEnvironmentMap, ViewDimension::TextureCube, 4);
        }
        return;
    }

    GPUScopedProfileCapture sampleMarker(pCmdList, L"Pathtracing");

    const ResolutionInfo& resInfo = GetFramework()->GetResolutionInfo();
    CameraComponent* pCamera = GetScene()->GetCurrentCamera();

    TraceRaysConstants constants = {};
    memcpy(constants.clip_to_world, &pCamera->GetInverseViewProjection(), sizeof(constants.clip_to_world));
    memcpy(constants.camera_to_world, &pCamera->GetInverseView(), sizeof(constants.camera_to_world));
    constants.inv_render_size[0] = 1.0f / resInfo.RenderWidth;
    constants.inv_render_size[1] = 1.0f / resInfo.RenderHeight;
    constants.frame_index = static_cast<uint32_t>(GetFramework()->GetFrameID());
    constants.ibl_factor = GetScene()->GetIBLFactor();
    constants.fuse_mode = m_pDenoiserRenderModule->GetFuseMode();
    constants.use_dominant_light = m_pDenoiserRenderModule->UseDominantLightVisibility() ? 1 : 0;
    constants.dominant_light_index = 0;

    const SceneLightingInformation& sceneLightInfo = GetScene()->GetSceneLightInfo();
    for (uint32_t i = 0; i < static_cast<uint32_t>(sceneLightInfo.LightCount); ++i)
    {
        if (sceneLightInfo.LightInfo[i].Type == static_cast<uint32_t>(LightType::Directional))
        {
            constants.dominant_light_index = i;
            break;
        }
    }

    BufferAddressInfo constantsBufferInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(TraceRaysConstants), reinterpret_cast<const void*>(&constants));
    m_pTraceRaysDenoiserParameterSet->UpdateRootConstantBuffer(&constantsBufferInfo, 0);

    BufferAddressInfo sceneBufferInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(SceneInformation), reinterpret_cast<const void*>(&GetScene()->GetSceneInfo()));
    m_pTraceRaysDenoiserParameterSet->UpdateRootConstantBuffer(&sceneBufferInfo, 1);

    BufferAddressInfo lightingBufferInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(sceneLightInfo), reinterpret_cast<const void*>(&sceneLightInfo));
    m_pTraceRaysDenoiserParameterSet->UpdateRootConstantBuffer(&lightingBufferInfo, 2);

    LightingCBData lightingConstantData;
    lightingConstantData.IBLFactor = GetScene()->GetIBLFactor();
    lightingConstantData.SpecularIBLFactor = GetScene()->GetSpecularIBLFactor();
    BufferAddressInfo lightingConstantDataInfo = GetDynamicBufferPool()->AllocConstantBuffer(sizeof(LightingCBData), reinterpret_cast<const void*>(&lightingConstantData));
    m_pTraceRaysDenoiserParameterSet->UpdateRootConstantBuffer(&lightingConstantDataInfo, 3);

    ShadowMapResourcePool* pShadowMapResourcePool = GetFramework()->GetShadowMapResourcePool();
    CauldronAssert(ASSERT_CRITICAL,
                   pShadowMapResourcePool->GetRenderTargetCount() <= MAX_SHADOW_MAP_TEXTURES_COUNT,
                   L"NRCRenderModule can only support up to %d shadow maps. There are currently %d shadow maps",
                   MAX_SHADOW_MAP_TEXTURES_COUNT,
                   pShadowMapResourcePool->GetRenderTargetCount());
    for (uint32_t i = 0; i < pShadowMapResourcePool->GetRenderTargetCount(); ++i)
    {
        m_pTraceRaysDenoiserParameterSet->SetTextureSRV(pShadowMapResourcePool->GetRenderTarget(i), ViewDimension::Texture2D, SHADOW_MAP_BEGIN_SLOT + i);
    }

    m_pTraceRaysDenoiserParameterSet->Bind(pCmdList, m_pTraceRaysDenoiserPipeline);
    SetPipelineState(pCmdList, m_pTraceRaysDenoiserPipeline);
    const uint32_t numGroupsX = (resInfo.RenderWidth + 7) / 8;
    const uint32_t numGroupsY = (resInfo.RenderHeight + 7) / 8;
    Dispatch(pCmdList, numGroupsX, numGroupsY, 1);
}

void PathTracingRenderModule::BuildUI()
{
    // noop
}

void PathTracingRenderModule::OnNewContentLoaded(cauldron::ContentBlock* pContentBlock)
{
    std::lock_guard<std::mutex> pipelineLock(m_Mutex);
 
    for (Material* pMat : pContentBlock->Materials)
    {
        PTMaterialInfo materialInfo = {};

        const Vec4 albedoColor = pMat->GetAlbedoColor();
        materialInfo.albedo_factor_x = albedoColor.getX();
        materialInfo.albedo_factor_y = albedoColor.getY();
        materialInfo.albedo_factor_z = albedoColor.getZ();
        materialInfo.albedo_factor_w = albedoColor.getW();

        const Vec4 emissiveColor = pMat->GetEmissiveColor();
        materialInfo.emission_factor_x = emissiveColor.getX();
        materialInfo.emission_factor_y = emissiveColor.getY();
        materialInfo.emission_factor_z = emissiveColor.getZ();

        const Vec4 pbrInfo = pMat->GetPBRInfo();
        materialInfo.arm_factor_x = 1.0f;
        materialInfo.arm_factor_y = pbrInfo.getY();
        materialInfo.arm_factor_z = pbrInfo.getX();

        materialInfo.is_opaque = pMat->GetBlendMode() == MaterialBlend::Opaque;
        materialInfo.alpha_cutoff = pMat->GetAlphaCutOff();
        materialInfo.is_double_sided = pMat->HasDoubleSided();

        int32_t samplerIndex;
        if (pMat->HasPBRInfo())
        {
            materialInfo.albedo_tex_id = AddTexture(pMat, TextureClass::Albedo, samplerIndex);
            materialInfo.albedo_tex_sampler_id = samplerIndex;

            if (pMat->HasPBRMetalRough())
            {
                materialInfo.arm_tex_id = AddTexture(pMat, TextureClass::MetalRough, samplerIndex);
                materialInfo.arm_tex_sampler_id = samplerIndex;
            }
            else if (pMat->HasPBRSpecGloss())
            {
                materialInfo.arm_tex_id = AddTexture(pMat, TextureClass::SpecGloss, samplerIndex);
                materialInfo.arm_tex_sampler_id = samplerIndex;
            }
        }

        materialInfo.normal_tex_id = AddTexture(pMat, TextureClass::Normal, samplerIndex);
        materialInfo.normal_tex_sampler_id = samplerIndex;
        materialInfo.emission_tex_id = AddTexture(pMat, TextureClass::Emissive, samplerIndex);
        materialInfo.emission_tex_sampler_id = samplerIndex;

        m_rtInfoTables.m_cpuMaterialBuffer.push_back(materialInfo);
    }

    MeshComponentMgr* pMeshComponentManager = MeshComponentMgr::Get();

    uint32_t nodeID = 0, surfaceID = 0;
    for (EntityDataBlock* pEntityData : pContentBlock->EntityDataBlocks)
    {
        for (Component* pComponent : pEntityData->Components)
        {
            if (pComponent->GetManager() == pMeshComponentManager)
            {
                PTInstanceInfo instanceInfo{};
                instanceInfo.surface_id_table_offset  = (uint32_t)m_rtInfoTables.m_cpuSurfaceIDsBuffer.size();
                const Mesh*  pMesh                    = reinterpret_cast<MeshComponent*>(pComponent)->GetData().pMesh;
                const size_t numSurfaces              = pMesh->GetNumSurfaces();
                size_t       numOpaqueSurfaces        = 0;

                for (uint32_t i = 0; i < numSurfaces; ++i)
                {
                    const Surface*  pSurface  = pMesh->GetSurface(i);
                    const Material* pMaterial = pSurface->GetMaterial();

                    m_rtInfoTables.m_cpuSurfaceIDsBuffer.push_back(surfaceID++);

                    PTSurfaceInfo surfaceInfo{};
                    memset(&surfaceInfo, -1, sizeof(surfaceInfo));
                    surfaceInfo.num_indices   = pSurface->GetIndexBuffer().Count;
                    surfaceInfo.num_vertices = pSurface->GetVertexBuffer(VertexAttributeType::Position).Count;

                    int foundIndex = -1;
                    for (size_t i = 0; i < m_rtInfoTables.m_IndexBuffers.size(); i++)
                    {
                        if (m_rtInfoTables.m_IndexBuffers[i] == pSurface->GetIndexBuffer().pBuffer)
                        {
                            foundIndex = (int)i;
                            break;
                        }
                    }

                    surfaceInfo.index_offset = foundIndex >= 0 ? foundIndex : (int)m_rtInfoTables.m_IndexBuffers.size();
                    if (foundIndex < 0)
                        m_rtInfoTables.m_IndexBuffers.push_back(pSurface->GetIndexBuffer().pBuffer);

                    switch (pSurface->GetIndexBuffer().IndexFormat)
                    {
                    case ResourceFormat::R16_UINT:
                        surfaceInfo.index_type = SURFACE_INFO_INDEX_TYPE_U16;
                        break;
                    case ResourceFormat::R32_UINT:
                        surfaceInfo.index_type = SURFACE_INFO_INDEX_TYPE_U32;
                        break;
                    default:
                        CauldronError(L"Unsupported resource format for ray tracing indices");
                    }

                    uint32_t usedAttributes = VertexAttributeFlag_Position | VertexAttributeFlag_Normal | VertexAttributeFlag_Tangent |
                                              VertexAttributeFlag_Texcoord0 | VertexAttributeFlag_Texcoord1;

                    const uint32_t surfaceAttributes = pSurface->GetVertexAttributes();
                    usedAttributes = usedAttributes & surfaceAttributes;

                    for (uint32_t attribute = 0; attribute < static_cast<uint32_t>(VertexAttributeType::Count); ++attribute)
                    {
                        // Check if the attribute is present
                        if (usedAttributes & (0x1 << attribute))
                        {
                            int foundIndex = -1;
                            for (size_t i = 0; i < m_rtInfoTables.m_VertexBuffers.size(); i++)
                            {
                                if (m_rtInfoTables.m_VertexBuffers[i] == pSurface->GetVertexBuffer(static_cast<VertexAttributeType>(attribute)).pBuffer)
                                {
                                    foundIndex = (int)i;
                                    break;
                                }
                            }
                            if (foundIndex < 0)
                                m_rtInfoTables.m_VertexBuffers.push_back(pSurface->GetVertexBuffer(static_cast<VertexAttributeType>(attribute)).pBuffer);
                            switch (static_cast<VertexAttributeType>(attribute))
                            {
                            case cauldron::VertexAttributeType::Position:
                                surfaceInfo.position_attribute_offset = foundIndex >= 0 ? foundIndex : (int)m_rtInfoTables.m_VertexBuffers.size() - 1;
                                break;
                            case cauldron::VertexAttributeType::Normal:
                                surfaceInfo.normal_attribute_offset = foundIndex >= 0 ? foundIndex : (int)m_rtInfoTables.m_VertexBuffers.size() - 1;
                                break;
                            case cauldron::VertexAttributeType::Tangent:
                                surfaceInfo.tangent_attribute_offset = foundIndex >= 0 ? foundIndex : (int)m_rtInfoTables.m_VertexBuffers.size() - 1;
                                break;
                            case cauldron::VertexAttributeType::Texcoord0:
                                surfaceInfo.texcoord0_attribute_offset = foundIndex >= 0 ? foundIndex : (int)m_rtInfoTables.m_VertexBuffers.size() - 1;
                                break;
                            case cauldron::VertexAttributeType::Texcoord1:
                                surfaceInfo.texcoord1_attribute_offset = foundIndex >= 0 ? foundIndex : (int)m_rtInfoTables.m_VertexBuffers.size() - 1;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    for (size_t i = 0; i < pContentBlock->Materials.size(); i++)
                    {
                        if (pContentBlock->Materials[i] == pMaterial)
                        {
                            surfaceInfo.material_id = (uint32_t)i;
                            break;
                        }
                    }
                    m_rtInfoTables.m_cpuSurfaceBuffer.push_back(surfaceInfo);

                    if (!pSurface->HasTranslucency())
                        numOpaqueSurfaces++;
                }

                instanceInfo.num_surfaces        = (uint32_t)(numOpaqueSurfaces);
                instanceInfo.num_opaque_surfaces = (uint32_t)(numSurfaces);
                instanceInfo.node_id             = nodeID++;
                m_rtInfoTables.m_cpuInstanceBuffer.push_back(instanceInfo);
            }
        }
    }

    if (m_rtInfoTables.m_cpuSurfaceBuffer.size() > 0)
    {
        // Upload
        BufferDesc bufferMaterial = BufferDesc::Data(
            L"PTMaterialBuffer", uint32_t(m_rtInfoTables.m_cpuMaterialBuffer.size() * sizeof(PTMaterialInfo)), sizeof(PTMaterialInfo), 0, ResourceFlags::None);
        m_rtInfoTables.m_pMaterialBuffer = GetDynamicResourcePool()->CreateBuffer(&bufferMaterial, ResourceState::CopyDest);
        const_cast<Buffer*>(m_rtInfoTables.m_pMaterialBuffer)
            ->CopyData(m_rtInfoTables.m_cpuMaterialBuffer.data(), m_rtInfoTables.m_cpuMaterialBuffer.size() * sizeof(PTMaterialInfo));

        BufferDesc bufferInstance = BufferDesc::Data(
            L"PTInstanceBuffer", uint32_t(m_rtInfoTables.m_cpuInstanceBuffer.size() * sizeof(PTInstanceInfo)), sizeof(PTInstanceInfo), 0, ResourceFlags::None);
        m_rtInfoTables.m_pInstanceBuffer = GetDynamicResourcePool()->CreateBuffer(&bufferInstance, ResourceState::CopyDest);
        const_cast<Buffer*>(m_rtInfoTables.m_pInstanceBuffer)
            ->CopyData(m_rtInfoTables.m_cpuInstanceBuffer.data(), m_rtInfoTables.m_cpuInstanceBuffer.size() * sizeof(PTInstanceInfo));

        BufferDesc bufferSurfaceID = BufferDesc::Data(
            L"PTSurfaceIDBuffer", uint32_t(m_rtInfoTables.m_cpuSurfaceIDsBuffer.size() * sizeof(uint32_t)), sizeof(uint32_t), 0, ResourceFlags::None);
        m_rtInfoTables.m_pSurfaceIDsBuffer = GetDynamicResourcePool()->CreateBuffer(&bufferSurfaceID, ResourceState::CopyDest);
        const_cast<Buffer*>(m_rtInfoTables.m_pSurfaceIDsBuffer)
            ->CopyData(m_rtInfoTables.m_cpuSurfaceIDsBuffer.data(), m_rtInfoTables.m_cpuSurfaceIDsBuffer.size() * sizeof(uint32_t));

        BufferDesc bufferSurface = BufferDesc::Data(
            L"PTSurfaceBuffer", uint32_t(m_rtInfoTables.m_cpuSurfaceBuffer.size() * sizeof(PTSurfaceInfo)), sizeof(PTSurfaceInfo), 0, ResourceFlags::None);
        m_rtInfoTables.m_pSurfaceBuffer = GetDynamicResourcePool()->CreateBuffer(&bufferSurface, ResourceState::CopyDest);
        const_cast<Buffer*>(m_rtInfoTables.m_pSurfaceBuffer)
            ->CopyData(m_rtInfoTables.m_cpuSurfaceBuffer.data(), m_rtInfoTables.m_cpuSurfaceBuffer.size() * sizeof(PTSurfaceInfo));

        m_pTraceRaysDenoiserParameterSet->SetBufferSRV(m_rtInfoTables.m_pMaterialBuffer, RAYTRACING_INFO_BEGIN_SLOT);
        m_pTraceRaysDenoiserParameterSet->SetBufferSRV(m_rtInfoTables.m_pInstanceBuffer, RAYTRACING_INFO_BEGIN_SLOT + 1);
        m_pTraceRaysDenoiserParameterSet->SetBufferSRV(m_rtInfoTables.m_pSurfaceIDsBuffer, RAYTRACING_INFO_BEGIN_SLOT + 2);
        m_pTraceRaysDenoiserParameterSet->SetBufferSRV(m_rtInfoTables.m_pSurfaceBuffer, RAYTRACING_INFO_BEGIN_SLOT + 3);
    }

    {
        // Update the parameter set with loaded texture entries
        CauldronAssert(ASSERT_CRITICAL, m_rtInfoTables.m_Textures.size() <= MAX_TEXTURES_COUNT, L"Too many textures.");
        for (uint32_t i = 0; i < m_rtInfoTables.m_Textures.size(); ++i)
        {
            m_pTraceRaysDenoiserParameterSet->SetTextureSRV(m_rtInfoTables.m_Textures[i].pTexture, ViewDimension::Texture2D, i + TEXTURE_BEGIN_SLOT);
        }

        // Update sampler bindings as well
        CauldronAssert(ASSERT_CRITICAL, m_rtInfoTables.m_Samplers.size() <= MAX_SAMPLERS_COUNT, L"Too many samplers.");
        for (uint32_t i = 0; i < m_rtInfoTables.m_Samplers.size(); ++i)
        {
            m_pTraceRaysDenoiserParameterSet->SetSampler(m_rtInfoTables.m_Samplers[i], i + SAMPLER_BEGIN_SLOT);
        }

        CauldronAssert(ASSERT_CRITICAL, m_rtInfoTables.m_IndexBuffers.size() <= MAX_BUFFER_COUNT, L"Too many index buffers.");
        for (uint32_t i = 0; i < m_rtInfoTables.m_IndexBuffers.size(); ++i)
        {
            m_pTraceRaysDenoiserParameterSet->SetBufferSRV(m_rtInfoTables.m_IndexBuffers[i], i + INDEX_BUFFER_BEGIN_SLOT);
        }

        CauldronAssert(ASSERT_CRITICAL, m_rtInfoTables.m_VertexBuffers.size() <= MAX_BUFFER_COUNT, L"Too many vertex buffers.");
        for (uint32_t i = 0; i < m_rtInfoTables.m_VertexBuffers.size(); ++i)
        {
            m_pTraceRaysDenoiserParameterSet->SetBufferSRV(m_rtInfoTables.m_VertexBuffers[i], i + VERTEX_BUFFER_BEGIN_SLOT);
        }
    }
}

void PathTracingRenderModule::OnContentUnloaded(cauldron::ContentBlock* pContentBlock)
{
    for (const PTMaterialInfo& materialInfo : m_rtInfoTables.m_cpuMaterialBuffer)
    {
        if (materialInfo.albedo_tex_id > 0)
            RemoveTexture(materialInfo.albedo_tex_id);
        if (materialInfo.arm_tex_id > 0)
            RemoveTexture(materialInfo.arm_tex_id);
        if (materialInfo.emission_tex_id > 0)
            RemoveTexture(materialInfo.emission_tex_id);
        if (materialInfo.normal_tex_id > 0)
            RemoveTexture(materialInfo.normal_tex_id);
    }
}

bool PathTracingRenderModule::InitPipelineObjects()
{
    auto AddTraceRaysRootSignatureSrvs = [this](RootSignatureDesc& rootSignatureDesc) {
            rootSignatureDesc.AddConstantBufferView(0, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddConstantBufferView(1, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddConstantBufferView(2, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddConstantBufferView(3, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddRTAccelerationStructureSet(0, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddTextureSRVSet(1, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddTextureSRVSet(2, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddTextureSRVSet(3, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddTextureSRVSet(4, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddBufferSRVSet(RAYTRACING_INFO_BEGIN_SLOT, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddBufferSRVSet(RAYTRACING_INFO_BEGIN_SLOT + 1, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddBufferSRVSet(RAYTRACING_INFO_BEGIN_SLOT + 2, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddBufferSRVSet(RAYTRACING_INFO_BEGIN_SLOT + 3, ShaderBindStage::Compute, 1);
            rootSignatureDesc.AddBufferSRVSet(INDEX_BUFFER_BEGIN_SLOT, ShaderBindStage::Compute, MAX_BUFFER_COUNT);
            rootSignatureDesc.AddBufferSRVSet(VERTEX_BUFFER_BEGIN_SLOT, ShaderBindStage::Compute, MAX_BUFFER_COUNT);
            rootSignatureDesc.AddTextureSRVSet(SHADOW_MAP_BEGIN_SLOT, ShaderBindStage::Compute, MAX_SHADOW_MAP_TEXTURES_COUNT);
            rootSignatureDesc.AddTextureSRVSet(TEXTURE_BEGIN_SLOT, ShaderBindStage::Compute, MAX_TEXTURES_COUNT);
            rootSignatureDesc.AddStaticSamplers(0, ShaderBindStage::Compute, 1, &m_SpecularSampler);
            rootSignatureDesc.AddStaticSamplers(1, ShaderBindStage::Compute, 1, &m_DiffuseSampler);
            rootSignatureDesc.AddStaticSamplers(2, ShaderBindStage::Compute, 1, &m_SpecularSampler);
            rootSignatureDesc.AddStaticSamplers(3, ShaderBindStage::Compute, 1, &m_ComparisonSampler);
            rootSignatureDesc.AddSamplerSet(SAMPLER_BEGIN_SLOT, ShaderBindStage::Compute, MAX_SAMPLERS_COUNT);
        };

    {
        RootSignatureDesc rootSignatureDesc;
        AddTraceRaysRootSignatureSrvs(rootSignatureDesc);

        rootSignatureDesc.AddTextureUAVSet(0, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(1, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(2, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(3, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(4, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(5, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(6, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(7, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(8, ShaderBindStage::Compute, 1);
        rootSignatureDesc.AddTextureUAVSet(9, ShaderBindStage::Compute, 1);

        m_pTraceRaysDenoiserRootSignature = RootSignature::CreateRootSignature(L"TraceRaysDenoiser_RootSignature", rootSignatureDesc);
        if (!m_pTraceRaysDenoiserRootSignature)
            return false;

        PipelineDesc pipelineDesc;
        pipelineDesc.SetRootSignature(m_pTraceRaysDenoiserRootSignature);
        ShaderBuildDesc shaderDesc = ShaderBuildDesc::Compute(L"trace_rays_denoiser.hlsl", L"main", ShaderModel::SM6_6, nullptr);
        pipelineDesc.AddShaderDesc(shaderDesc);
        m_pTraceRaysDenoiserPipeline = PipelineObject::CreatePipelineObject(L"TraceRaysDenoiser_Pipeline", pipelineDesc);
        if (!m_pTraceRaysDenoiserPipeline)
            return false;

        m_pTraceRaysDenoiserParameterSet = ParameterSet::CreateParameterSet(m_pTraceRaysDenoiserRootSignature);
        m_pTraceRaysDenoiserParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(TraceRaysConstants), 0);
        m_pTraceRaysDenoiserParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(SceneInformation), 1);
        m_pTraceRaysDenoiserParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(SceneLightingInformation), 2);
        m_pTraceRaysDenoiserParameterSet->SetRootConstantBufferResource(GetDynamicBufferPool()->GetResource(), sizeof(LightingCBData), 3);
        m_pTraceRaysDenoiserParameterSet->SetAccelerationStructure(GetScene()->GetASManager()->GetTLAS(), 0);

        ShadowMapResourcePool* pShadowMapResourcePool = GetFramework()->GetShadowMapResourcePool();
        for (uint32_t i = 0; i < pShadowMapResourcePool->GetRenderTargetCount(); ++i)
        {
            m_pTraceRaysDenoiserParameterSet->SetTextureSRV(pShadowMapResourcePool->GetRenderTarget(i), ViewDimension::Texture2D, SHADOW_MAP_BEGIN_SLOT + i);
        }
    }

    return true;
}

bool PathTracingRenderModule::InitResources()
{
    uint32_t renderWidth  = GetFramework()->GetResolutionInfo().RenderWidth;
    uint32_t renderHeight = GetFramework()->GetResolutionInfo().RenderHeight;
    auto renderSizeFn = [](TextureDesc& desc, uint32_t displayWidth, uint32_t displayHeight, uint32_t renderingWidth, uint32_t renderingHeight) {
        desc.Width  = renderingWidth;
        desc.Height = renderingHeight;
    };

    m_pColorTarget = GetFramework()->GetColorTargetForCallback(GetName());
    m_pDepthTarget = GetFramework()->GetRenderTexture(L"DepthTarget");
    m_pDirectDiffuseOutput = GetFramework()->GetRenderTexture(L"DenoiserDirectDiffuseTarget");
    m_pDirectSpecularOutput = GetFramework()->GetRenderTexture(L"DenoiserDirectSpecularTarget");
    m_pIndirectDiffuseOutput = GetFramework()->GetRenderTexture(L"DenoiserIndirectDiffuseTarget");
    m_pIndirectSpecularOutput = GetFramework()->GetRenderTexture(L"DenoiserIndirectSpecularTarget");
    m_pDominantLightVisibilityOutput = GetFramework()->GetRenderTexture(L"DenoiserDominantLightVisibilityTarget");

    m_pDiffuseAlbedo = GetFramework()->GetRenderTexture(L"DenoiserDiffuseAlbedoTarget");
    m_pSpecularAlbedo = GetFramework()->GetRenderTexture(L"DenoiserSpecularAlbedoTarget");
    m_pFusedAlbedo = GetFramework()->GetRenderTexture(L"DenoiserFusedAlbedoTarget");
    m_pNormals = GetFramework()->GetRenderTexture(L"DenoiserNormalsTarget");
    m_pSkipSignal = GetFramework()->GetRenderTexture(L"DenoiserSkipSignalTarget");

    m_pTraceRaysDenoiserParameterSet->SetTextureSRV(m_pDepthTarget, ViewDimension::Texture2D, 1);

    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pDirectSpecularOutput, ViewDimension::Texture2D, 0);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pDirectDiffuseOutput, ViewDimension::Texture2D, 1);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pIndirectSpecularOutput, ViewDimension::Texture2D, 2);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pIndirectDiffuseOutput, ViewDimension::Texture2D, 3);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pDominantLightVisibilityOutput, ViewDimension::Texture2D, 4);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pDiffuseAlbedo, ViewDimension::Texture2D, 5);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pSpecularAlbedo, ViewDimension::Texture2D, 6);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pFusedAlbedo, ViewDimension::Texture2D, 7);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pNormals, ViewDimension::Texture2D, 8);
    m_pTraceRaysDenoiserParameterSet->SetTextureUAV(m_pSkipSignal, ViewDimension::Texture2D, 9);

    return true;
}

// Add texture index info and return the index to the texture in the texture array
int32_t PathTracingRenderModule::AddTexture(const Material* pMaterial, const TextureClass textureClass, int32_t& textureSamplerIndex)
{
    const cauldron::TextureInfo* pTextureInfo = pMaterial->GetTextureInfo(textureClass);
    if (pTextureInfo != nullptr)
    {
        // Check if the texture's sampler is already one we have, and if not add it
        for (textureSamplerIndex = 0; textureSamplerIndex < m_rtInfoTables.m_Samplers.size(); ++textureSamplerIndex)
        {
            if (m_rtInfoTables.m_Samplers[textureSamplerIndex]->GetDesc() == pTextureInfo->TexSamplerDesc)
                break;  // found
        }

        // If we didn't find the sampler, add it
        if (textureSamplerIndex == m_rtInfoTables.m_Samplers.size())
        {
            Sampler* pSampler = Sampler::CreateSampler(L"PTSampler", pTextureInfo->TexSamplerDesc);
            CauldronAssert(ASSERT_WARNING, pSampler, L"Could not create sampler for loaded content %s", pTextureInfo->pTexture->GetDesc().Name.c_str());
            m_rtInfoTables.m_Samplers.push_back(pSampler);
        }

        // Find a slot for the texture
        int32_t firstFreeIndex = -1;
        for (int32_t i = 0; i < m_rtInfoTables.m_Textures.size(); ++i)
        {
            RTInfoTables::BoundTexture& boundTexture = m_rtInfoTables.m_Textures[i];

            // If this texture is already mapped, bump it's reference count
            if (pTextureInfo->pTexture == boundTexture.pTexture)
            {
                boundTexture.count += 1;
                return i;
            }

            // Try to re-use an existing entry that was released
            else if (firstFreeIndex < 0 && boundTexture.count == 0)
            {
                firstFreeIndex = i;
            }
        }

        // Texture wasn't found
        RTInfoTables::BoundTexture b = {pTextureInfo->pTexture, 1};
        if (firstFreeIndex < 0)
        {
            m_rtInfoTables.m_Textures.push_back(b);
            return static_cast<int32_t>(m_rtInfoTables.m_Textures.size()) - 1;
        }
        else
        {
            m_rtInfoTables.m_Textures[firstFreeIndex] = b;
            return firstFreeIndex;
        }
    }
    return -1;
}

void PathTracingRenderModule::RemoveTexture(int32_t index)
{
    if (index >= 0)
    {
        m_rtInfoTables.m_Textures[index].count -= 1;
        if (m_rtInfoTables.m_Textures[index].count == 0)
        {
            m_rtInfoTables.m_Textures[index].pTexture = nullptr;
        }
    }
}
