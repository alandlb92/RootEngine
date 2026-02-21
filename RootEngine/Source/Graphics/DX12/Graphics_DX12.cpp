#include "Graphics/DX12/Graphics_DX12.h"
#include <assert.h>
#include "Faia/Debug.h"
#include "Faia/WindowsApplication.h"
#include <d3dcompiler.h>
#include "d3d12.h"
#include "dxgi.h"

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
            float positions_test[18] = {
                -0.8f, 0.4f,0.5f,
                0.8f, 0.4f,0.5f,
                0.8f, -0.4f,0.5f,
                -0.8f, 0.4f,0.5f,
                0.8f, -0.4f,0.5f,
                -0.8f, -0.4f,0.5f
            };

            float colors_test[24] = {
                1.f, 0.f, 0.f, 1.f,
                0.f, 1.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
                1.f, 0.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
                1.f, 0.f, 1.f, 1.f
            };
            
            void Graphics_DX12::SetupDevice()
			{
				Debug::Log("Setup device DX12");
                CalculateWindowSize();
				CreateDeviceAndFence();
				CreateCommandQueueAllocatorAndList();
                CreateDXGIFactoryAndSwapChain();
                CreateRenderTargetView();
                CreateDepthStencilView();
                CreateAndSetRootSignature();
                SetViewportAndScissorRects();
                ResourceBarrierRenderTarget(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET); 
                Clear();
                SetViewportAndScissorRects();
                SetRenderTargets();
                SetupInputLayout();
                LoadVS();
                SetVertexBuffer();
                LoadPS();
                CreateAndSetPipelineState();
                CommitAndPresent();
			}


            void Graphics_DX12::CalculateWindowSize()
            {
                assert(Windows::GetWindowHandler() != 0);
                RECT clientRect;
                GetClientRect(Windows::GetWindowHandler(), &clientRect);
                mClientWidth = clientRect.right - clientRect.left;
                mClientHeight = clientRect.bottom - clientRect.top;
            }

            void Graphics_DX12::CreateDeviceAndFence()
			{
				//Creating Device
				HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice));
				if (FAILED(hr))
				{
					Debug::PopError("Error on create device");
				}

				//Creating fence    
				hr = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
				if (FAILED(hr))
				{
					Debug::PopError("Error on create FENCE");
				}
			}

            void Graphics_DX12::CreateCommandQueueAllocatorAndList()
            {
                D3D12_COMMAND_QUEUE_DESC queueDesc = {};
                queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
                queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

                HRESULT hr = mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
                if (FAILED(hr))
                {
					Debug::PopError("Error on CreateCommandQueue");
                }

                //Command Allocator
                hr = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator));
                if (FAILED(hr))
                {
					Debug::PopError("Error on CreateCommandAllocator");
                }

                //Command List
                hr = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator, nullptr, IID_PPV_ARGS(&mCommandList));
                if (FAILED(hr))
                {
					Debug::PopError("Error on CreateCommandList");
                }
            }

            void Graphics_DX12::CreateDXGIFactoryAndSwapChain()
            {
                //Create dxgifactory
                IDXGIFactory* mDxgiFactory;
                CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));

                //Swap chain
                DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
                swapChainDesc.BufferDesc.Width = mClientWidth;
                swapChainDesc.BufferDesc.Height = mClientHeight;
                swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
                swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
                swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
                swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
                swapChainDesc.SampleDesc.Count = 1;
                swapChainDesc.SampleDesc.Quality = 0;
                swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                swapChainDesc.BufferCount = SwapChainBufferCount;
                swapChainDesc.OutputWindow = Windows::GetWindowHandler();
                swapChainDesc.Windowed = true;
                swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
                swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


                HRESULT hr = mDxgiFactory->CreateSwapChain(mCommandQueue, &swapChainDesc, &mSwapChain);
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateSwapChain");
                }
            }

            void Graphics_DX12::CreateRenderTargetView()
            {
                //Render target view heap    
                D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
                RTVHeapDesc.NumDescriptors = SwapChainBufferCount;
                RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                RTVHeapDesc.NodeMask = 0;
                HRESULT hr = mDevice->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mRTVHeap));
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateDescriptorHeap RTV");
                }
                //Get Descriptor size
                UINT mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


                //Create render target view
                D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());
                for (UINT i = 0; i < SwapChainBufferCount; i++)
                {
                    hr = mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i]));
                    if (FAILED(hr))
                    {
                        Debug::PopError("Error on GetBuffer from Swapchain");
                    }
                    
                    mDevice->CreateRenderTargetView(mSwapChainBuffer[i], nullptr, rtvHeapHandle);

                    rtvHeapHandle.ptr += mRtvDescriptorSize;
                }
            }
            
            void Graphics_DX12::CreateDepthStencilView()
            {
                D3D12_RESOURCE_DESC depthStencilDesc = {};
                depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                depthStencilDesc.Alignment = 0;
                depthStencilDesc.Width = mClientWidth;
                depthStencilDesc.Height = mClientHeight;
                depthStencilDesc.DepthOrArraySize = 1;
                depthStencilDesc.MipLevels = 0;
                depthStencilDesc.Format = DephtStencilFormat;
                depthStencilDesc.SampleDesc.Count = 1;
                depthStencilDesc.SampleDesc.Quality = 0;
                depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
                depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

                D3D12_CLEAR_VALUE optClear;
                optClear.Format = DephtStencilFormat;
                optClear.DepthStencil.Depth = 1.0f;
                optClear.DepthStencil.Stencil = 0;

                D3D12_HEAP_PROPERTIES heapProperties = {};
                heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
                heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
                heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
                heapProperties.CreationNodeMask = NULL;
                heapProperties.VisibleNodeMask = NULL;
                HRESULT hr = mDevice->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc,
                    D3D12_RESOURCE_STATE_DEPTH_WRITE, &optClear, IID_PPV_ARGS(&mDepthStencilBuffer));
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateCommittedResource Depth/Stencil");
                }


                D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc = {};
                DSVHeapDesc.NumDescriptors = 1;
                DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
                DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                hr = mDevice->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&mDSVHeap));
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateDescriptorHeap DSV");
                }


                D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
                dsvDesc.Format = DephtStencilFormat;
                dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
                dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
                dsvDesc.Texture2D.MipSlice = 0;
                D3D12_CPU_DESCRIPTOR_HANDLE heapHandleDsv = mDSVHeap->GetCPUDescriptorHandleForHeapStart();
                mDevice->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, heapHandleDsv);
            }

            void Graphics_DX12::CreateAndSetRootSignature()
            {
                //Creating Root signature
                D3D12_ROOT_PARAMETER rootParameters[1] = {};
                rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
                rootParameters[0].Descriptor.ShaderRegister = 0;
                rootParameters[0].Descriptor.RegisterSpace = 0;

                D3D12_ROOT_SIGNATURE_DESC  rootSignDesc = {};
                rootSignDesc.NumParameters = _countof(rootParameters);
                rootSignDesc.pParameters = rootParameters;
                rootSignDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

                ID3DBlob* signatureBlob;
                ID3DBlob* errorBlob;
                HRESULT hr = D3D12SerializeRootSignature(&rootSignDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
                if (FAILED(hr))
                {
                    Debug::PopError("Error on D3D12SerializeVersionedRootSignature");
                }
                hr = mDevice->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateRootSignature");
                }

                mCommandList->SetGraphicsRootSignature(mRootSignature);
            }
            
            void Graphics_DX12::SetViewportAndScissorRects()
            {
                D3D12_VIEWPORT viewport = {};
                viewport.TopLeftX = 0.0f;
                viewport.TopLeftY = 0.0f;
                viewport.Width = static_cast<float>(mClientWidth);
                viewport.Height = static_cast<float>(mClientHeight);
                viewport.MinDepth = 0.f;
                viewport.MaxDepth = 1.f;

                mScissorRect = { 0, 0, static_cast<LONG>(mClientWidth), static_cast<LONG>(mClientHeight) };

                mCommandList->RSSetViewports(1, &viewport);
                mCommandList->RSSetScissorRects(1, &mScissorRect);
            }

            void Graphics_DX12::ResourceBarrierRenderTarget(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
            {
                // Transition the back buffer to render target
                D3D12_RESOURCE_BARRIER bufferRenderTargetBarrier = {};
                bufferRenderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                bufferRenderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                bufferRenderTargetBarrier.Transition.pResource = mSwapChainBuffer[0]; // Assuming using first buffer
                bufferRenderTargetBarrier.Transition.StateBefore = before;
                bufferRenderTargetBarrier.Transition.StateAfter = after;
                bufferRenderTargetBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                mCommandList->ResourceBarrier(1, &bufferRenderTargetBarrier);
            }

            void Graphics_DX12::Clear()
            {
                D3D12_CPU_DESCRIPTOR_HANDLE rtvHandler = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
                D3D12_CPU_DESCRIPTOR_HANDLE dsvHandler = mDSVHeap->GetCPUDescriptorHandleForHeapStart();
                float clearColor[4] = { 0.2f, 0.3f, 0.4f, 1.0f };
                mCommandList->ClearDepthStencilView(dsvHandler, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
                mCommandList->ClearRenderTargetView(rtvHandler, clearColor, 0, nullptr);
            }

            void Graphics_DX12::SetRenderTargets()
            {
                D3D12_CPU_DESCRIPTOR_HANDLE rtvHandler = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
                D3D12_CPU_DESCRIPTOR_HANDLE dsvHandler = mDSVHeap->GetCPUDescriptorHandleForHeapStart();
                mCommandList->OMSetRenderTargets(1, &rtvHandler, FALSE, &dsvHandler);
            }

            void Graphics_DX12::LoadVS()
            {
                //Vertex shader
                ID3DBlob* vsBlob;
                HRESULT hr = D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);
                if (FAILED(hr))
                {
                    Debug::PopError("Fail to load VS");
                }

                mShaderByteCodeVS = {};
                mShaderByteCodeVS.pShaderBytecode = vsBlob->GetBufferPointer();
                mShaderByteCodeVS.BytecodeLength = vsBlob->GetBufferSize();
            }

            void Graphics_DX12::SetupInputLayout()
            {
                mInputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
                mInputLayout[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

                mInputLayoutDesc = {};
                mInputLayoutDesc.pInputElementDescs = mInputLayout;
                mInputLayoutDesc.NumElements = 2;
            }

            void Graphics_DX12::SetVertexBuffer()
            {
                //Set POSITION vertex Buffer
                ID3D12Resource* posVertexBuffer;
                D3D12_HEAP_PROPERTIES heapProps = {};
                heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

                D3D12_RESOURCE_DESC posResourceDesc = {};
                posResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
                posResourceDesc.Width = sizeof(positions_test);
                posResourceDesc.Height = 1;
                posResourceDesc.DepthOrArraySize = 1;
                posResourceDesc.MipLevels = 1;
                posResourceDesc.SampleDesc.Count = 1;
                posResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

                HRESULT hr = mDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &posResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&posVertexBuffer));
                if (FAILED(hr))
                {
                    Debug::PopError("Fail to CreateCommittedResource");
                }

                //Copy data to Vertex buffer
                void* pPosVertexDataBegin;
                D3D12_RANGE readRange = { 0, 0 };
                posVertexBuffer->Map(0, &readRange, &pPosVertexDataBegin);
                memcpy(pPosVertexDataBegin, positions_test, sizeof(positions_test));
                posVertexBuffer->Unmap(0, nullptr);

                //Buffer view
                mPosVertexBufferView = {};
                mPosVertexBufferView.BufferLocation = posVertexBuffer->GetGPUVirtualAddress();
                mPosVertexBufferView.SizeInBytes = sizeof(positions_test);
                mPosVertexBufferView.StrideInBytes = sizeof(float) * 3;

                //Set COLOR vertex Buffer
                D3D12_RESOURCE_DESC colorResourceDesc = posResourceDesc;
                colorResourceDesc.Width = sizeof(colors_test);
                ID3D12Resource* colorVertexBuffer;
                hr = mDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &colorResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&colorVertexBuffer));
                if (FAILED(hr))
                {
                    Debug::PopError("Fail to CreateCommittedResource");
                }

                //Copy data to Vertex buffer
                void* pColorsVertexDataBegin;
                D3D12_RANGE colorReadRange = { 0, 0 };
                colorVertexBuffer->Map(0, &colorReadRange, &pColorsVertexDataBegin);
                memcpy(pColorsVertexDataBegin, colors_test, sizeof(colors_test));
                colorVertexBuffer->Unmap(0, nullptr);

                //Buffer view
                mColorVertexBufferView = {};
                mColorVertexBufferView.BufferLocation = colorVertexBuffer->GetGPUVirtualAddress();
                mColorVertexBufferView.SizeInBytes = sizeof(colors_test);
                mColorVertexBufferView.StrideInBytes = sizeof(float) * 4;

                mCommandList->IASetVertexBuffers(0, 1, &mPosVertexBufferView);
                mCommandList->IASetVertexBuffers(1, 1, &mColorVertexBufferView);
            }
            
            void Graphics_DX12::LoadPS()
            {
                //Pixel shader
                ID3DBlob* psBlob;
                HRESULT hr = D3DReadFileToBlob(L"PixelShader.cso", &psBlob);
                if (FAILED(hr))
                {
                    Debug::PopError("Fail to load PS");
                }

                mShaderByteCodePS = {};
                mShaderByteCodePS.pShaderBytecode = psBlob->GetBufferPointer();
                mShaderByteCodePS.BytecodeLength = psBlob->GetBufferSize();
            }

            void Graphics_DX12::CreateAndSetPipelineState()
            {
                D3D12_RASTERIZER_DESC rasterizerDesc = {};
                rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
                rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
                rasterizerDesc.FrontCounterClockwise = false;
                rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
                rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
                rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
                rasterizerDesc.DepthClipEnable = TRUE;
                rasterizerDesc.MultisampleEnable = FALSE;
                rasterizerDesc.AntialiasedLineEnable = FALSE;
                rasterizerDesc.ForcedSampleCount = 0;
                rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


                D3D12_DEPTH_STENCIL_DESC depthDesc = {};
                depthDesc.DepthEnable = TRUE;
                depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
                depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
                depthDesc.StencilEnable = FALSE;

                D3D12_BLEND_DESC blendDesc = {};
                blendDesc.AlphaToCoverageEnable = FALSE;
                blendDesc.IndependentBlendEnable = FALSE;
                blendDesc.RenderTarget[0].BlendEnable = FALSE;
                blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
                blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
                blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
                blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
                blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
                blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
                blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
                blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
                blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

                D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc = {};
                psDesc.InputLayout = mInputLayoutDesc;
                psDesc.pRootSignature = mRootSignature;
                psDesc.VS = mShaderByteCodeVS;
                psDesc.PS = mShaderByteCodePS;
                psDesc.RasterizerState = rasterizerDesc;
                psDesc.BlendState = blendDesc;
                psDesc.NumRenderTargets = 1;
                psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                psDesc.DSVFormat = DephtStencilFormat;
                psDesc.DepthStencilState = depthDesc;
                psDesc.SampleDesc.Count = 1;
                psDesc.SampleDesc.Quality = 0;
                psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                psDesc.SampleMask = UINT_MAX;
                HRESULT hr = mDevice->CreateGraphicsPipelineState(&psDesc, IID_PPV_ARGS(&mPipelineState));
                if (FAILED(hr))
                {
                    Debug::PopError("Fail to CreateGraphicsPipelineState");
                }

                mCommandList->SetPipelineState(mPipelineState);
            }

            void Graphics_DX12::CommitAndPresent()
            {
                mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                mCommandList->DrawInstanced(6, 1, 0, 0);
                ResourceBarrierRenderTarget(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
                mCommandList->Close();
                ID3D12CommandList* ppCommandLists[] = { mCommandList };
                mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
                mSwapChain->Present(false, 0);
            }
		}
	}
}