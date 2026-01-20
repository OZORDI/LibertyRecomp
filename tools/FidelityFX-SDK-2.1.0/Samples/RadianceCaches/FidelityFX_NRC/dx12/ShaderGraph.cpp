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

#include "ShaderGraph.h"
#include "utils/FilesystemUtils.h"
#include "utils/StringUtils.h"
#include "utils/HighResTimer.h"
#include <regex>

#include "utils/Json.h"

#include <OpenSource/nlohmann/json.hpp>

namespace fsr
{
    LiveShader::LiveShader(const std::string& filePath, DXC& dxcCtx, ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> rootSignature) :
        m_filePath(filePath),
        m_isDirty(false),
        m_shaderState(kUnloaded),
        m_device(device),
        m_rootSignature(rootSignature),
        m_dxcCtx(dxcCtx)
    {
    }

    void LiveShader::OnUpdateAsyc()
    {
        if (m_isDirty && m_shaderState != kEnqueued)
        {
            Compile();
            m_isDirty = false;
        }
    }

    void LiveShader::Destroy()
    {
        ReleaseResource(m_activePipeState);
        ReleaseResource(m_enqueuedPipeState);
    }

    void LiveShader::Compile()
    {
        Assert(FileExists(m_filePath));

        Log::Indent indent;
        Log::Write(Pad(50, ' ', "Recompiling '{}'...", GetFilename(m_filePath)) + '\b');       

        const int result = CompileImpl();        
        switch(result)
        {
            case DXC::kSucceeded: Log::Success("Okay."); break;
            case DXC::kSucceededWithWarnings: Log::Warning("Okay (with warnings)."); break;
            //case DXC::kFailed: Log::Error("Failed");
        }
    }

    ComPtr<ID3D12PipelineState>& LiveShader::GetPipelineState()
    {
        if (m_shaderState == kUnloaded) { return m_activePipeState; }

        // If a new state has been enqueued, release the currently active state and replace it
        if (m_shaderState == kEnqueued)
        {
            ReleaseResource(m_activePipeState);
            m_activePipeState = m_enqueuedPipeState;
            m_enqueuedPipeState.Detach();
            m_shaderState = kActive;
        }
        
        return m_activePipeState;
    }

    LivePixelShader::LivePixelShader(const std::string& filePath, const DXGI_FORMAT rtvFormat, DXC& dxcCtx, ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> rootSignature)
        : LiveShader(filePath, dxcCtx, device, rootSignature),
        m_rtvFormat(rtvFormat)
    {
    }

    int LivePixelShader::CompileImpl()
    {     
        static const std::string preambleBlock =
            "struct PSInput\n" \
            "{\n" \
            "    float4 position : SV_POSITION;\n" \
            "    float2 uv : TEXCOORD;\n" \
            "};\n";

        static const std::string vertShaderBlock = preambleBlock +
            "PSInput VSMain(float4 position : POSITION, float4 uv : TEXCOORD)\
            {\
                PSInput result;\
                result.position = position;\
                result.uv = uv.xy;\
                return result;\
            }\n";

        ComPtr<IDxcBlob> vsBlob;
        m_dxcCtx.CompileFromSource(vertShaderBlock.c_str(), L"VSMain", L"vs_6_0", vsBlob);
        Assert(vsBlob);

        ComPtr<IDxcBlob> psBlob;
        int compileResult = m_dxcCtx.CompileFromFile(m_filePath, L"PSMain", L"ps_6_0", psBlob);
        if(!psBlob)
        {        
            // If this shader has already been compiled, that's fine. Just keep using the old shader.
            if (m_shaderState != kUnloaded) { return compileResult; }

            // Otherwise, compile the null shader and continue as normal
            static const std::string nullPixShaderBlock =
                preambleBlock +
                "float4 PSMain(PSInput i) : SV_TARGET \
                { \
                    return float4(1, 0, 0, 1); \
                }";
            
            m_dxcCtx.CompileFromSource(nullPixShaderBlock.c_str(), L"PSMain", L"ps_6_0", psBlob);
            Assert(psBlob);
        }

        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        // Create the pipeline state
        D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
        gpsDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        gpsDesc.pRootSignature = m_rootSignature.Get();
        gpsDesc.VS = CD3DX12_SHADER_BYTECODE((ID3DBlob*)vsBlob.Get());
        gpsDesc.PS = CD3DX12_SHADER_BYTECODE((ID3DBlob*)psBlob.Get());
        gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        gpsDesc.DepthStencilState.DepthEnable = FALSE;
        gpsDesc.DepthStencilState.StencilEnable = FALSE;
        gpsDesc.SampleMask = UINT_MAX;
        gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        gpsDesc.NumRenderTargets = 1;
        gpsDesc.RTVFormats[0] = m_rtvFormat;
        gpsDesc.SampleDesc.Count = 1;

        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&m_enqueuedPipeState)));
        m_shaderState = kEnqueued;

        return compileResult;   
    }

    LiveComputeShader::LiveComputeShader(const std::string& filePath, DXC& dxcCtx, ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> rootSignature)
        : LiveShader(filePath, dxcCtx, device, rootSignature) {
    }

    int LiveComputeShader::CompileImpl()
    {
        ComPtr<IDxcBlob> csBlob;
        int compileResult = m_dxcCtx.CompileFromFile(m_filePath, L"CSMain", L"cs_6_6", csBlob);

        if (!csBlob)
        {
            // If this shader has already been compiled, that's fine. Just keep using the old shader.
            if (m_shaderState != kUnloaded) { return compileResult; }

            // Otherwise, compile the null shader and continue as normal
            static const std::string nullCompShaderBlock = "[numthreads(8, 8, 1)] void CSMain(uint3 threadId : SV_DispatchThreadID) {}";
            m_dxcCtx.CompileFromSource(nullCompShaderBlock.c_str(), L"CSMain", L"cs_6_6", csBlob);
            Assert(csBlob);
        }        

        // Create the pipeline state
        D3D12_COMPUTE_PIPELINE_STATE_DESC cpsDesc = {};
        cpsDesc.pRootSignature = m_rootSignature.Get();
        cpsDesc.CS = CD3DX12_SHADER_BYTECODE((ID3DBlob*)csBlob.Get());
        ThrowIfFailed(m_device->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&m_enqueuedPipeState)));
        m_shaderState = kEnqueued;

        return compileResult;
    }

    ShaderGraph::ShaderGraph(ComPtr<ID3D12Device> device, Json& dxcJson, const std::vector<std::string>& includeDirs) :
        m_threadStatus(kThreadStopped),
        m_device(device),
        m_dxcCtx(dxcJson)
    {
        m_fileIncludeDirs.push_back(DeslashifyPath(GetModuleDirectory()));        
        m_fileIncludeDirs.insert(m_fileIncludeDirs.begin(), includeDirs.begin(), includeDirs.end());

        m_dxcCtx.SetIncludeDirectories(m_fileIncludeDirs);
    }

    void ShaderGraph::Destroy()
    {
        m_threadStatus = kThreadShutdown;
        m_thread.join();
        
        for (auto& shader : m_shaders)
        {
            shader->Destroy();
            shader.reset();
        }
    }

    void ShaderGraph::Finalise()
    {
        AssertFmt(m_threadStatus == kThreadStopped, "Shader graph already finalized.");
        AssertFmt(!m_shaders.empty(), "No shaders to finalize.");

        // Compile the shaders
        Log::Write("Recompiling shaders...");
        for (auto& shader : m_shaders)
        {
            shader->Compile();
        }
        
        m_thread = std::thread(&ShaderGraph::MonitorDependencies, this);
    }

	void ShaderGraph::MonitorDependencies()
	{       
        m_threadStatus = kThreadRunning;
        constexpr int kShaderPollingInterval = 2000;
        constexpr int kThreadPollingInterval = 100;

        std::unordered_set<std::filesystem::path> badPaths;
        int elapsed = 0;
        while (m_threadStatus == kThreadRunning)
        {
            if (elapsed >= kShaderPollingInterval)
            {
                elapsed = 0;
                //std::printf("Poll...\n");

                // Check the write time on the files to see whether it's changed
                for (auto& file : m_shaderFiles)
                {
                    auto thisWriteTime = std::filesystem::last_write_time(std::filesystem::path(file.second.m_filePath));
                    if (thisWriteTime != file.second.m_prevWriteTime)
                    {
                        for (auto& ptr : file.second.m_deps)
                        {
                            Assert(!ptr.second.expired());
                            ptr.second.lock()->MakeDirty();
                        }
                        file.second.m_prevWriteTime = thisWriteTime;
                    }
                }

                std::vector<LiveShader*> dirtied;
                for (auto& ptr : m_shaders) 
                { 
                    if(ptr->IsDirty()) { dirtied.push_back(ptr.get()); }
                }

                // Reload any shaders that were dirtied
                if(!dirtied.empty())
                {
                    Log::Write("Reloading dirty shaders...");
                    for (auto& ptr : dirtied)
                    {
                        ptr->OnUpdateAsyc();
                    }
                }
            }

            elapsed += kThreadPollingInterval;
            std::this_thread::sleep_for(std::chrono::milliseconds(kThreadPollingInterval));
        };

        m_threadStatus = kThreadStopped;
	}

    // Tries to resolve the shader path from all include directories in order of precedence
    bool ShaderGraph::ResolveShaderPath(const std::string& path, const std::string& rootPath, std::string& resolvedPath) const
    {
        resolvedPath = path;
        if (FileExists(resolvedPath)) { return true; }

        const std::string filename = GetFilename(path);
        if (!rootPath.empty())
        {
            resolvedPath = GetParentDirectory(rootPath) + "\\" + filename;
            if (FileExists(resolvedPath)) { return true; }
        }

        // Next, try using implicit include directories...
        for (const auto& prefix : m_fileIncludeDirs)
        {
            resolvedPath = prefix + "/" + filename;
            if (FileExists(resolvedPath)) { return true; }
            resolvedPath = prefix + "/" + path;
            if (FileExists(resolvedPath)) { return true; }
        }

        return false;
    }

    void ShaderGraph::ParseShaderRecurse(const std::string& path, const std::string& rootPath, const int depth, CreateCtx& ctx)
    {
        AssertFmt(depth < 10, "Possible #include cycle detected in '%s'", rootPath.c_str());
        
        // Try loading the file from disk
        std::string resolvedPath;
        AssertFmt(ResolveShaderPath(path, rootPath, resolvedPath), "Error: shader '{}' not found in any include directory.", path.c_str());

        std::string codeBlock = ReadTextFile(resolvedPath);

        // If the shader object hasn't been intialised, auto-detect and create it
        if(!ctx.shader)
        {
            Assert(depth == 0);
            if(codeBlock.find("CSMain", 0) != std::string::npos)
            {                
                ctx.shader.reset(new LiveComputeShader(rootPath, m_dxcCtx, m_device, ctx.rootSignature));
            }
            else if (codeBlock.find("PSMain", 0) != std::string::npos)
            {
                ctx.shader.reset(new LivePixelShader(rootPath, DXGI_FORMAT_R8G8B8A8_UNORM, m_dxcCtx, m_device, ctx.rootSignature));
            }

            AssertMsg(ctx.shader, "Shader must contain either CSMain or PSMain entry points.");            
        }

        // Initialise the shader file entry
        auto& entry = m_shaderFiles[resolvedPath];
        entry.m_deps[rootPath] = ctx.shader;
        entry.m_filePath = resolvedPath;
        entry.m_prevWriteTime = std::filesystem::last_write_time(std::filesystem::path(resolvedPath));
        
        // Scrub any control codes from the input which could potentially throw off the compiler
        for (int i = 0; i < codeBlock.size(); ++i)
        {
            if (std::iscntrl(codeBlock[i]) && codeBlock[i] != '\n') { codeBlock[i] = ' '; }
        }

        auto RemoveToken = [](std::string& codeBlock, const std::string& token)
            {
                size_t offset = 0;
                while ((offset = codeBlock.find(token, offset)) != std::string::npos)
                {
                    for (int i = 0; i < token.length(); ++i, ++offset) { codeBlock[offset] = ' '; }
                }
            };

        // Inline embed code from #include blocks
        size_t offset = 0;
        do
        {
            // Parse the name of the included file
            offset = codeBlock.find("#include", offset);
            if (offset == std::string::npos) { break; }

            size_t startQuote = codeBlock.find("\"", offset);
            size_t endQuote = codeBlock.find("\"", startQuote + 1);
            if (startQuote == std::string::npos || endQuote == std::string::npos) { break; }
            
            const std::string includeFilename = codeBlock.substr(startQuote + 1, endQuote - startQuote - 1);
            if (includeFilename != "Builtin.hlsl")
            {
                ParseShaderRecurse(includeFilename, rootPath, depth + 1, ctx);
            }

            offset = endQuote + 1;

        } while (offset < codeBlock.length());
    }

    std::shared_ptr<LiveShader> ShaderGraph::CreateShader(const std::string& path, ComPtr<ID3D12RootSignature> rootSignature)
    {              
        std::string rootPath;
        AssertFmt(ResolveShaderPath(path, "", rootPath), "Error: shader '{}' not found in any include directory.", path.c_str());

        CreateCtx ctx;
        ctx.rootSignature = rootSignature;

        ParseShaderRecurse(path, rootPath, 0, ctx);

        m_shaders.push_back(ctx.shader);
        return ctx.shader;
    }
}
