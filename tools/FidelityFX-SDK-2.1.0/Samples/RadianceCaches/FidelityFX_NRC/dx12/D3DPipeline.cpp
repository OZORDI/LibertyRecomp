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

#include "D3DPipeline.h"

#include <windows.h>
#include <windowsx.h>
#include <string>

#include "utils/Log.h"
#include "utils/StringUtils.h"

#include <OpenSource/imgui/backends/imgui_impl_dx12.h>
#include <OpenSource/imgui/backends/imgui_impl_win32.h>

namespace fsr
{
	//template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	D3DPipeline::D3DPipeline(std::string name) :
		m_clientWidth(0),
		m_clientHeight(0),
		m_backBufferIdx(0),
		m_scissorRect(0, 0, 0, 0),
		m_fenceValues{ 0, 0 },
		m_rtvDescriptorSize(0),
		m_callbackCookie(0)
	{
		m_frame.idx = 0;
		m_frame.sumDeltaT = 0.;
		m_frame.sumN = 0.;
		m_viewport = { 0.0f, 0.0f, 0.0f, 0.0f, };
	}

	void D3DPipeline::OnCreate(HWND hWnd, const Json& json)
	{
		m_hWnd = hWnd;

		UpdateAssetDimensions();
		
		CreateDevice();

		CreateSwapChain();	

		CreateRenderTargets();

		CreateRootSignature();

		CreateSynchronisationObjects();

		CreateImguiObjects(m_rootSignature, m_device, 2);

		m_renderManager.reset(new RenderManager(m_device, m_commandList, m_clientWidth, m_clientHeight, json, m_deviceName));
		m_renderConfig = m_renderManager->GetConfig();

		// Close the command list and execute it to begin the initial GPU setup.
		ThrowIfFailed(m_commandList->Close());
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
		
		WaitForGpu();
	}

	void D3DPipeline::OnDestroy()
	{
		// Ensure that the GPU is no longer referencing resources that are about to be
		// cleaned up by the destructor.
		WaitForGpu();

		m_renderManager->Destroy();
		m_renderManager.reset();	

		DestroyImguiObjects();

		for (int i = 0; i < kFrameCount; ++i)
		{
			ReleaseResource(m_renderTargets[kFrameCount]);
			ReleaseResource(m_commandAllocators[kFrameCount]);
		}

		//ReleaseResource(m_triangleVertexBuffer);
		ReleaseResource(m_factory);
		ReleaseResource(m_swapChain);
		ReleaseResource(m_commandQueue);
		ReleaseResource(m_rootSignature);
		ReleaseResource(m_rtvHeap);
		ReleaseResource(m_srvHeap);
		ReleaseResource(m_pipelineState);
		ReleaseResource(m_trianglePipelineState);
		ReleaseResource(m_commandList);
		ReleaseResource(m_fence);
		CloseHandle(m_fenceEvent);

		ReleaseResource(m_device);
	}

	void D3DPipeline::CreateImguiObjects(ComPtr<ID3D12RootSignature>& rootSignature, ComPtr<ID3D12Device>& device, const int numConcurrentFrames)
	{
		Log::Write(Pad(50, ' ', "Initializing IMGUI...") + "\b");
		
		Assert(rootSignature);
		Assert(device);

		// Describe and create a shader resource view (SRV) heap for the texture.
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_imguiSrvHeap)));

		Assert(ImGui_ImplDX12_Init(device.Get(), numConcurrentFrames, DXGI_FORMAT_R8G8B8A8_UNORM, m_imguiSrvHeap.Get(),
			m_imguiSrvHeap->GetCPUDescriptorHandleForHeapStart(), m_imguiSrvHeap->GetGPUDescriptorHandleForHeapStart()));

		Assert(ImGui_ImplDX12_CreateDeviceObjects());

		Log::Success("Okay!\n");
	}

	void D3DPipeline::PopulateImguiCommandList(ComPtr<ID3D12GraphicsCommandList>& commandList, const int frameIdx)
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		const auto kBaseSize = ImGui::CalcTextSize("A");
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, kBaseSize.x * 2.0f);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.3f));

		auto HelpMarker = [](const char* text)
			{
				ImGui::TextDisabled("[?]");
				if (ImGui::BeginItemTooltip())
				{
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(text);
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}			
			};

		// Menu Bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Renderer"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{		
				//ImGui::MenuItem("Memory monitor", NULL, &m_showMemoryMonitor);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Settings
		if (ImGui::Begin("Settings", nullptr, 0))
		{
			bool update = false;		

			if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				
				update |= ImGui::Checkbox("Animate", &m_renderConfig.animate.enable);
				ImGui::Indent();
				update |= ImGui::Checkbox("Materials", &m_renderConfig.animate.materials);
				update |= ImGui::Checkbox("Geometry", &m_renderConfig.animate.geometry);
				update |= ImGui::Checkbox("Lights", &m_renderConfig.animate.lights);
				update |= ImGui::Checkbox("Camera", &m_renderConfig.animate.camera);
				ImGui::Unindent();

				ImGui::Spacing();
			
				ImGui::Text("Split screen");
				ImGui::Indent();
				if(m_renderConfig.display.demoMode) { ImGui::BeginDisabled(); }				
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
				update |= ImGui::SliderFloat("##", &m_renderConfig.display.splitPartition, 0.f, 1.f, "##");
				ImGui::PopItemWidth();
				if (m_renderConfig.display.demoMode) { ImGui::EndDisabled(); }
				ImGui::SameLine(); 
				update |= ImGui::Checkbox("Demo mode", &m_renderConfig.display.demoMode);
				ImGui::SameLine();
				HelpMarker("Drag to adjust the partition between radiance cache and reference modes.\n"
					"Left-hand side: radiance cache.\n"
					"Right-hand side: reference");
				ImGui::Unindent();

				ImGui::Spacing();

				ImGui::Text("Settings");
				ImGui::Indent();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
				update |= ImGui::DragFloat("Motion blur", &m_renderConfig.renderer.accumBlur, 0.01f, 0.0f, 1.0f);
				update |= ImGui::DragFloat("Indirect roughening", &m_renderConfig.renderer.indirectRoughening, 0.01f, 0.0f, 1.0f);
				ImGui::SameLine();
				HelpMarker("Roughens surfaces for indirect rays. Reduces fireflies.");
				ImGui::PopItemWidth();
				update |= ImGui::Checkbox("Lock noise", &m_renderConfig.renderer.lockNoise);
				ImGui::Unindent();

				ImGui::Unindent();
			}

			ImGui::Spacing();

			/*********************************************************************************************/

			if (ImGui::CollapsingHeader("Radiance Cache", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
				ImGui::InputFloat("Learning rate", &m_renderConfig.cache.learningRate, 1e-3f, 1e-2f, "%.6f");
				m_renderConfig.cache.learningRate = std::clamp(m_renderConfig.cache.learningRate, 1e-6f, 1.f);
				ImGui::InputFloat("Weight smoothing", &m_renderConfig.cache.weightSmoothing, 1e-2f, 1e-1f, "%.3f");
				m_renderConfig.cache.weightSmoothing = std::clamp(m_renderConfig.cache.weightSmoothing, 0.f, 1.f);
				ImGui::PopItemWidth();

				auto ImGuiComboWrapper = [](const char** items, const int numItems, int& selectedIdx)
					{
						const char* previewValue = items[selectedIdx];
						if (ImGui::BeginCombo("Optimizer", previewValue, 0))
						{
							for (int itemIdx = 0; itemIdx < numItems; itemIdx++)
							{
								const bool isSelected = (selectedIdx == itemIdx);
								if (ImGui::Selectable(items[itemIdx], isSelected)) { selectedIdx = itemIdx; }
								if (isSelected) { ImGui::SetItemDefaultFocus(); }
							}
							ImGui::EndCombo();
						}
					};

				if (ImGui::Button("Update"))
				{
					update = true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Defaults"))
				{
					m_renderConfig = RenderManagerConfig();
					update = true;
				}

				m_renderConfig.resetCache = false;
				if (ImGui::Button("Reset cache"))
				{
					m_renderConfig.resetCache = true;
					update = true;
				}

				ImGui::Separator();

				ImGui::Text("Sample occupancy");
				if (ImGui::BeginTable("occupancy", 4, ImGuiTableFlags_BordersH))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn(); ImGui::Text("Enqueued");
					ImGui::TableNextColumn(); ImGui::Text("Max");
					ImGui::TableNextColumn(); ImGui::Text("%%");

					for (const auto& stat : m_stats.bufferOccupancy)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text(std::get<0>(stat).c_str());
						ImGui::TableNextColumn();
						ImGui::Text("%i", std::get<1>(stat));
						ImGui::TableNextColumn();
						ImGui::Text("%i", std::get<2>(stat));
						ImGui::TableNextColumn();
						ImGui::Text("%.2f%%", 100 * float(std::get<1>(stat)) / float(std::get<2>(stat)));
					}
					ImGui::EndTable();
				}

				ImGui::Unindent();
			}

			ImGui::Spacing();

			/*********************************************************************************************/

			if (ImGui::CollapsingHeader("Performance Stats", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent();
				
				if (ImGui::BeginTable("perftable", 2))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Backend: ");
					ImGui::TableNextColumn();
					if (m_renderManager->IsWMMAEnabled()) { ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "WMMA"); }
					else { ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.0f, 1.0f), "Reference [SLOW]"); }

					for (const auto& stat : m_stats.layerPerf)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text(stat.layerId.c_str());
						ImGui::TableNextColumn();
						ImGui::Text("%.3fms", stat.timeMs);
					}
					ImGui::EndTable();
				}

				ImGui::Unindent();
			}			

			if (update) { m_renderManager->UpdateConfig(m_renderConfig); }

		} 
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		// Rendering
		ImGui::Render();

		auto drawData = ImGui::GetDrawData();
		if (!drawData) { return; }

		ID3D12DescriptorHeap* ppHeaps[] = { m_imguiSrvHeap.Get() };
		commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		ImGui_ImplDX12_RenderDrawData(drawData, commandList.Get());
	}

	void D3DPipeline::DestroyImguiObjects()
	{
		ImGui_ImplDX12_Shutdown();

		ReleaseResource(m_imguiSrvHeap);

		Log::Write("Destroyed IMGUI D3D objects.\n");
	}

	void D3DPipeline::UpdateAssetDimensions()
	{
		// Update the window dimensions
		RECT clientRect;
		GetClientRect(m_hWnd, &clientRect);
		m_clientWidth = clientRect.right;
		m_clientHeight = clientRect.bottom;
		m_viewport = D3D12_VIEWPORT{ 0.0f, 0.0f, float(m_clientWidth), float(m_clientHeight) };
		m_scissorRect = CD3DX12_RECT(0, 0, LONG(m_clientWidth), LONG(m_clientHeight));

		// Calcualate nearest power-of-two values for the texture
		m_quadTexWidth = m_clientWidth;
		m_quadTexHeight = m_clientHeight;

		Log::System("D3D quad texture: %i x %i", m_quadTexWidth, m_quadTexHeight);
	}

	void D3DPipeline::OnUpdate() {}

	void D3DMessageCallback(D3D12_MESSAGE_CATEGORY category, D3D12_MESSAGE_SEVERITY severity, D3D12_MESSAGE_ID id, LPCSTR pDescription, void* pContext)
	{
		switch(severity)
		{
		case D3D12_MESSAGE_SEVERITY_ERROR:
			Log::Error("D3D error: {}", pDescription); break;
		case D3D12_MESSAGE_SEVERITY_WARNING:
			Log::Warning("D3D warning: {}", pDescription); break;
		default:
			Log::Debug("D3D message: {}", pDescription); break;
		}
	}

	void D3DPipeline::CreateDevice()
	{
		UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();

				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif

		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory)));

		{
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(m_factory.Get(), &hardwareAdapter);

			DXGI_ADAPTER_DESC adapterDesc;
			hardwareAdapter->GetDesc(&adapterDesc);

			ThrowIfFailed(D3D12CreateDevice(
				hardwareAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_device)
			));
			DXGI_ADAPTER_DESC1 desc;
			hardwareAdapter->GetDesc1(&desc);
			m_dx12deviceluid = desc.AdapterLuid;
			m_deviceName = Narrow(adapterDesc.Description);
		}

		if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(&m_infoQueue))))
		{
			// Break on errors/corruption
			//m_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			m_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);

			// Set callback for messages
			m_infoQueue->RegisterMessageCallback(&D3DMessageCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &m_callbackCookie);
		}

		// Describe and create the command queue.
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

		// Create descriptor heaps.
		{
			// Describe and create a render target view (RTV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = kFrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

			// Describe and create a shader resource view (SRV) heap for the texture.
			D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
			srvHeapDesc.NumDescriptors = 1;
			srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			ThrowIfFailed(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));

			m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		// Create the command list.
		{
			// Create command allocator for each frame.
			for (UINT n = 0; n < kFrameCount; n++)
			{
				ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
			}

			ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
		}
	}

	void D3DPipeline::CreateRenderTargets()
	{
		// Create frame resources.
		{
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV and a command allocator for each frame.
			for (UINT n = 0; n < kFrameCount; n++)
			{
				ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
				m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
				rtvHandle.ptr += m_rtvDescriptorSize;
			}
		}
	}

	void D3DPipeline::CreateSwapChain()
	{
		// Describe and create the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = kFrameCount;
		swapChainDesc.Width = m_clientWidth;
		swapChainDesc.Height = m_clientHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		ComPtr<IDXGISwapChain1> swapChain;
		ThrowIfFailed(m_factory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),		// Swap chain needs the queue so that it can force a flush on it.
			m_hWnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		));

		// This sample does not support fullscreen transitions.
		ThrowIfFailed(m_factory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(swapChain.As(&m_swapChain));
		m_backBufferIdx = m_swapChain->GetCurrentBackBufferIndex();
	}

	// Load the rendering pipeline dependencies.
	void D3DPipeline::CreatePipeline()
	{
		
	}

	void D3DPipeline::CreateRootSignature()
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE);

		CD3DX12_ROOT_PARAMETER1 rootParameters[1];
		rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
		ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}

	void D3DPipeline::CreateSynchronisationObjects()
	{
		// Create synchronization objects and wait until assets have been uploaded to the GPU.
		{
			ThrowIfFailed(m_device->CreateFence(m_fenceValues[m_backBufferIdx], D3D12_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_fence)));

			// Create an event handle to use for frame synchronization.
			m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (m_fenceEvent == nullptr)
			{
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
			}

			//m_moduleManager->LinkSynchronisationObjects(m_device, m_fence, m_fenceEvent);

			m_fenceValues[m_backBufferIdx]++;

			// Wait for the command list to execute; we are reusing the same command 
			// list in our main loop but for now, we just want to wait for setup to 
			// complete before continuing.
			WaitForGpu();
		}
	}

	void D3DPipeline::PopulateCommandList()
	{
		// Command list allocators can only be reset when the associated 
		// command lists have finished execution on the GPU; apps should use 
		// fences to determine GPU execution progress.
		ThrowIfFailed(m_commandAllocators[m_backBufferIdx]->Reset());

		// However, when ExecuteCommandList() is called on a particular command 
		// list, that command list can then be reset at any time and must be before 
		// re-recording.
		ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_backBufferIdx].Get(), nullptr));

		//m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

		//ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap.Get() };
		//m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// Set necessary state.
		//m_commandList->SetGraphicsRootDescriptorTable(0, m_srvHeap->GetGPUDescriptorHandleForHeapStart());
		m_commandList->RSSetViewports(1, &m_viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate that the back buffer will be used as a render target.
		CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIdx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &resBarrier);

		///////////////////////////////////

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIdx, m_rtvDescriptorSize);

		if (m_renderManager)
		{
			m_renderManager->PopulateCommandList(m_commandList, rtvHandle);
		}

		///////////////////////////////////

		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		PopulateImguiCommandList(m_commandList, m_backBufferIdx);
		//if (m_ui) { m_ui->PopulateCommandList(m_commandList, m_backBufferIdx); }

		///////////////////////////////////

		resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIdx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_commandList->ResourceBarrier(1, &resBarrier);

		ThrowIfFailed(m_commandList->Close());
	}

	// Render the scene.
	void D3DPipeline::OnRender()
	{
		//Log::Debug("Frame {}", ++m_frameIdx);

		m_frame.timer.Reset();

		// Record all the commands we need to render the scene into the command list.
		PopulateCommandList();

		// Execute the command list.
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame.
		ThrowIfFailed(m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
		//ThrowIfFailed(m_swapChain->Present(1, 0));

		// Schedule a Signal command in the queue.
		const UINT64 currentFenceValue = m_fenceValues[m_backBufferIdx];
		ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

		// After everything's rendered, dispatch any commands that IMGUI may have emitted
		//m_ui.DispatchRenderCommands();

		//m_moduleManager->UpdateD3DOutputTexture(m_fenceValues[m_backBufferIdx]);
		//m_commandQueue->Signal(m_fence.Get(), currentFenceValue + 1);

		// Update the frame index.
		m_backBufferIdx = m_swapChain->GetCurrentBackBufferIndex();

		// If the next frame is not ready to be rendered yet, wait until it is ready.
		if (m_fence->GetCompletedValue() < m_fenceValues[m_backBufferIdx])
		{
			//std::printf("%i is waiting for %i (%i)\n", m_backBufferIdx, m_fenceValues[m_backBufferIdx], currentFenceValue);
			ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_backBufferIdx], m_fenceEvent));
			WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
		}		

		m_stats = m_renderManager->GatherStats(m_commandQueue);

		// Calcualte the smoothed framerate
		m_frame.sumDeltaT = m_frame.sumDeltaT * 0.99f + m_frame.timer.Get();
		m_frame.sumN = m_frame.sumN * 0.99f + 1;
		const float deltaT = m_frame.sumDeltaT / m_frame.sumN;

		std::wstring title = std::format(L"FSR Radiance Cache - {} fps ({:.2f}ms)", int(1.f / deltaT), deltaT * 1e3f);
 		SetWindowText(m_hWnd, title.c_str());

		if (m_infoQueue && m_callbackCookie)
		{
			m_infoQueue->UnregisterMessageCallback(m_callbackCookie);
			m_callbackCookie = 0;
		}

		// Set the fence value for the next frame.
		m_fenceValues[m_backBufferIdx] = currentFenceValue + 2;

		//std::printf("Frame: %i\n", m_backBufferIdx);
	}

	// Wait for pending GPU work to complete.
	void D3DPipeline::WaitForGpu()
	{
		// Schedule a Signal command in the queue.
		ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_backBufferIdx]));

		// Wait until the fence has been processed.
		ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_backBufferIdx], m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

		// Increment the fence value for the current frame.
		m_fenceValues[m_backBufferIdx]++;
	}

	_Use_decl_annotations_
		void D3DPipeline::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		*ppAdapter = adapter.Detach();
	}

	void D3DPipeline::OnClientResize(HWND hWnd, UINT width, UINT height, WPARAM wParam)
	{
		if (!m_device || wParam == SIZE_MINIMIZED) { return; }

		//WaitForGpu();


		/*	ImGui_ImplDX12_InvalidateDeviceObjects();
			CleanupRenderTarget();
			ResizeSwapChain(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			CreateRenderTarget();
			ImGui_ImplDX12_CreateDeviceObjects();
		}
		return 0;*/
	}

	void D3DPipeline::OnFocusChange(HWND hWnd, bool isSet)
	{
		//if (m_moduleManager) { m_moduleManager->GetRenderer().FocusChange(isSet); }
	}

	void D3DPipeline::OnKey(const WPARAM code, const bool isSysKey, const bool isDown)
	{
		//if (m_moduleManager) { m_moduleManager->GetRenderer().SetKey(code, isSysKey, isDown); }
	}

	void D3DPipeline::OnMouseButton(const int button, const bool isDown)
	{
		//if (m_moduleManager) { m_moduleManager->GetRenderer().SetMouseButton(button, isDown); }
	}

	void D3DPipeline::OnMouseMove(const int mouseX, const int mouseY, const WPARAM flags)
	{
		//if (m_moduleManager) { m_moduleManager->GetRenderer().SetMousePos(mouseX, mouseY, flags); }
	}

	void D3DPipeline::OnMouseWheel(const float degrees)
	{
		//if (m_moduleManager) { m_moduleManager->GetRenderer().SetMouseWheel(degrees); }
	}
}
