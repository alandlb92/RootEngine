#include "Graphics/DX12/Graphics_DX12.h"
#include <assert.h>
#include "Faia/Debug.h"
#include "Faia/WindowsApplication.h"
#include <d3dcompiler.h>
#include "Faia/FErrors.h"

namespace Faia
{
	namespace Root
	{
        namespace Graphics
        {

            float positions_test[42] = {
                 -0.8f, 0.4f, 0.5f,     1.f, 0.f, 0.f, 1.f,
                 0.8f, 0.4f, 0.5f,     0.f, 1.f, 0.f, 1.f,
                 0.8f,-0.4f, 0.5f,     0.f, 0.f, 1.f, 1.f,
                -0.8f, 0.4f, 0.5f,     1.f, 0.f, 0.f, 1.f,
                 0.8f,-0.4f, 0.5f,     0.f, 0.f, 1.f, 1.f,
                -0.8f,-0.4f, 0.5f,     1.f, 0.f, 1.f, 1.f
            };

            float colors_test[24] = {
                1.f, 0.f, 0.f, 1.f,
                0.f, 1.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
                1.f, 0.f, 0.f, 1.f,
                0.f, 0.f, 1.f, 1.f,
                1.f, 0.f, 1.f, 1.f
            };

            void Graphics_DX12::SetupViewportAndScissorRects()
            {
                mViewport = {};
                mViewport.TopLeftX = .6f;
                mViewport.TopLeftY = .7f;
                mViewport.Width = static_cast<float>(.2f * mClientWidth);
                mViewport.Height = static_cast<float>(mClientHeight);
                mViewport.MinDepth = 0.f;
                mViewport.MaxDepth = 1.f;

                mScissorRect = { 0, 0, static_cast<LONG>(mClientWidth), static_cast<LONG>(mClientHeight) };
            }

            void Graphics_DX12::SetupDepthStencilView()
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
                hr = mDevice->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&mDsvHeap));
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateDescriptorHeap DSV");
                }


                D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
                dsvDesc.Format = DephtStencilFormat;
                dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
                dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
                dsvDesc.Texture2D.MipSlice = 0;
                D3D12_CPU_DESCRIPTOR_HANDLE heapHandleDsv = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
                mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, heapHandleDsv);
            }


            void Graphics_DX12::Setup()
            {
                CalculateWindowSize();
                SetupViewportAndScissorRects();

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

                ComPtr<IDXGIFactory4> factory;
                ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

                if (mUseWarpDevice)
                {
                    ComPtr<IDXGIAdapter> warpAdapter;
                    ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

                    ThrowIfFailed(D3D12CreateDevice(
                        warpAdapter.Get(),
                        D3D_FEATURE_LEVEL_11_0,
                        IID_PPV_ARGS(&mDevice)
                    ));
                }
                else
                {
                    ComPtr<IDXGIAdapter1> hardwareAdapter;
                    GetHardwareAdapter(factory.Get(), &hardwareAdapter);

                    ThrowIfFailed(D3D12CreateDevice(
                        hardwareAdapter.Get(),
                        D3D_FEATURE_LEVEL_11_0,
                        IID_PPV_ARGS(&mDevice)
                    ));
                }
                // Describe and create the command queue.
                D3D12_COMMAND_QUEUE_DESC queueDesc = {};
                queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
                queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

                ThrowIfFailed(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

                // Describe and create the swap chain.
                DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
                swapChainDesc.BufferCount = FrameCount;
                swapChainDesc.Width = mClientWidth;
                swapChainDesc.Height = mClientHeight;
                swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
                swapChainDesc.SampleDesc.Count = 1;

                ComPtr<IDXGISwapChain1> swapChain;
                ThrowIfFailed(factory->CreateSwapChainForHwnd(
                    mCommandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
                    Windows::GetWindowHandler(),
                    &swapChainDesc,
                    nullptr,
                    nullptr,
                    &swapChain
                ));

                // This sample does not support fullscreen transitions.
                ThrowIfFailed(factory->MakeWindowAssociation(Windows::GetWindowHandler(), DXGI_MWA_NO_ALT_ENTER));

                ThrowIfFailed(swapChain.As(&mSwapChain));
                mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

                // Create descriptor heaps.
                {
                    // Describe and create a render target view (RTV) descriptor heap.
                    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
                    rtvHeapDesc.NumDescriptors = FrameCount;
                    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                    ThrowIfFailed(mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));

                    mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                }

                // Create frame resources.
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());

                    // Create a RTV for each frame.
                    for (UINT n = 0; n < FrameCount; n++)
                    {
                        ThrowIfFailed(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n])));
                        mDevice->CreateRenderTargetView(mRenderTargets[n].Get(), nullptr, rtvHandle);
                        //rtvHandle.Offset(1, mRtvDescriptorSize);
                        rtvHandle.ptr += mRtvDescriptorSize;
                    }
                }

                SetupDepthStencilView();
                SetupMainDescriptorHeap();

                for (int i = 0; i < FrameCount; i++)
                {
                    ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator[i])));
                }


                LoadAssets();

                // Signal once during initialization so the first BeginFrame() wait
                // has a valid fence value to check against.
                mCommandQueue->Signal(mFence.Get(), mCurrentFenceValue);
                mFenceValue[mFrameIndex] = mCurrentFenceValue;
            }

            void Graphics_DX12::BeginFrame()
            {
                mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
                WaitForPreviousFrame();
                // Record all the commands we need to render the scene into the command list.
                PopulateCommandList();
            }

            void Graphics_DX12::EndFrame()
            {
                // Indicate that the back buffer will now be used to present.
                PrepareResourceBarrierRenderTarget(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
                //m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

                ThrowIfFailed(mCommandList->Close());
                // Execute the command list.
                
                ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
                mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

                // Signal                
                mCommandQueue->Signal(mFence.Get(), mCurrentFenceValue);
                mFenceValue[mFrameIndex] = mCurrentFenceValue;
                mCurrentFenceValue++;
                // Present the frame.
                ThrowIfFailed(mSwapChain->Present(0, 0));                
            }

            void Graphics_DX12::PrepareResourceBarrierRenderTarget(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
            {
                // Transition the back buffer to render target
                D3D12_RESOURCE_BARRIER bufferRenderTargetBarrier = {};
                bufferRenderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                bufferRenderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                bufferRenderTargetBarrier.Transition.pResource = mRenderTargets[mFrameIndex].Get(); // Assuming using first buffer
                bufferRenderTargetBarrier.Transition.StateBefore = before;
                bufferRenderTargetBarrier.Transition.StateAfter = after;
                bufferRenderTargetBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                mCommandList->ResourceBarrier(1, &bufferRenderTargetBarrier);
            }

            void Graphics_DX12::SetupRootSignature()
            {
                D3D12_DESCRIPTOR_RANGE descriptorRange = {};
                descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                descriptorRange.NumDescriptors = 1;
                descriptorRange.BaseShaderRegister = 0; // t0
                descriptorRange.RegisterSpace = 0;
                descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

                //Creating Root signature
                D3D12_ROOT_PARAMETER rootParameters[2] = {};
                rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
                rootParameters[0].Descriptor.ShaderRegister = 0;
                rootParameters[0].Descriptor.RegisterSpace = 0;

                rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
                rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
                rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

                D3D12_ROOT_SIGNATURE_DESC  rootSignDesc = {};
                rootSignDesc.NumParameters = _countof(rootParameters);
                rootSignDesc.pParameters = rootParameters;
                rootSignDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

                ID3DBlob* signatureBlob;
                ID3DBlob* errorBlob;
                ThrowIfFailed(D3D12SerializeRootSignature(&rootSignDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob));
                ThrowIfFailed(mDevice->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
            }

            void Graphics_DX12::SetupMainDescriptorHeap()
            {
                D3D12_DESCRIPTOR_HEAP_DESC MainHeapDesc = {};
                MainHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                MainHeapDesc.NumDescriptors = 100;
                MainHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
                MainHeapDesc.NodeMask = 0;

                HRESULT hr = mDevice->CreateDescriptorHeap(&MainHeapDesc, IID_PPV_ARGS(&mCBVSRVUAVHeap));
                if (FAILED(hr))
                {
                    Debug::PopError("Error on CreateDescriptorHeap Main");
                }
            }



            void Graphics_DX12::LoadShader(LPCWSTR FileName, D3D12_SHADER_BYTECODE& ShaderByteCode)
            {
                //Vertex shader
                ID3DBlob* vsBlob;
                ThrowIfFailed(D3DReadFileToBlob(FileName, &vsBlob));

                ShaderByteCode = {};
                ShaderByteCode.pShaderBytecode = vsBlob->GetBufferPointer();
                ShaderByteCode.BytecodeLength = vsBlob->GetBufferSize();
            }

            // Load the sample assets.
            void Graphics_DX12::LoadAssets()
            {
                // Create an empty root signature.
                SetupRootSignature();

                // Create the pipeline state, which includes compiling and loading shaders.
                {
                    D3D12_SHADER_BYTECODE shaderByteCodeVS;
                    D3D12_SHADER_BYTECODE shaderByteCodePS;
                    LoadShader(L"VertexShader.cso", shaderByteCodeVS);
                    LoadShader(L"PixelShader.cso", shaderByteCodePS);

                    // Define the vertex input layout.
                    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
                    {
                        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
                    };

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

                    // Describe and create the graphics pipeline state object (PSO).
                    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
                    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
                    psoDesc.pRootSignature = mRootSignature.Get();
                    psoDesc.VS = shaderByteCodeVS;
                    psoDesc.PS = shaderByteCodePS;
                    psoDesc.RasterizerState = rasterizerDesc;
                    psoDesc.BlendState = blendDesc;
                    psoDesc.DepthStencilState.DepthEnable = FALSE;
                    psoDesc.DepthStencilState.StencilEnable = FALSE;
                    psoDesc.SampleMask = UINT_MAX;
                    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                    psoDesc.NumRenderTargets = 1;
                    psoDesc.RTVFormats[0] = RTVFormat;
                    psoDesc.SampleDesc.Count = 1;
                    ThrowIfFailed(mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState)));
                }

                // Create the command list.
                ThrowIfFailed(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[mFrameIndex].Get(), mPipelineState.Get(), IID_PPV_ARGS(&mCommandList)));

                // Command lists are created in the recording state, but there is nothing
                // to record yet. The main loop expects it to be closed, so close it now.
                ThrowIfFailed(mCommandList->Close());

                // Create the vertex buffer.
                {
                    // Define the geometry for a triangle.
                 
                    const UINT vertexBufferSize = sizeof(positions_test);

                    // Note: using upload heaps to transfer static data like vert buffers is not 
                    // recommended. Every time the GPU needs it, the upload heap will be marshalled 
                    // over. Please read up on Default Heap usage. An upload heap is used here for 
                    // code simplicity and because there are very few verts to actually transfer.

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

                    ThrowIfFailed(mDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &posResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&posVertexBuffer)));

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
                    mPosVertexBufferView.StrideInBytes = sizeof(float) * 7;

                    ////Set COLOR vertex Buffer
                    //D3D12_RESOURCE_DESC colorResourceDesc = posResourceDesc;
                    //colorResourceDesc.Width = sizeof(colors_test);
                    //ID3D12Resource* colorVertexBuffer;
                    //ThrowIfFailed(mDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &colorResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&colorVertexBuffer)));

                    ////Copy data to Vertex buffer
                    //void* pColorsVertexDataBegin;
                    //D3D12_RANGE colorReadRange = { 0, 0 };
                    //colorVertexBuffer->Map(0, &colorReadRange, &pColorsVertexDataBegin);
                    //memcpy(pColorsVertexDataBegin, colors_test, sizeof(colors_test));
                    //colorVertexBuffer->Unmap(0, nullptr);

                    ////Buffer view
                    //mColorVertexBufferView = {};
                    //mColorVertexBufferView.BufferLocation = colorVertexBuffer->GetGPUVirtualAddress();
                    //mColorVertexBufferView.SizeInBytes = sizeof(colors_test);
                    //mColorVertexBufferView.StrideInBytes = sizeof(float) * 4;
                }


                // Create synchronization objects and wait until assets have been uploaded to the GPU.
                {
                    ThrowIfFailed(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
                    mFenceValue[mFrameIndex] = 1;

                    // Create an event handle to use for frame synchronization.
                    mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
                    if (mFenceEvent == nullptr)
                    {
                        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
                    }

                    // Wait for the command list to execute; we are reusing the same command 
                    // list in our main loop but for now, we just want to wait for setup to 
                    // complete before continuing.
                }
            }

            void Graphics_DX12::PopulateCommandList()
            {
                // Command list allocators can only be reset when the associated 
                // command lists have finished execution on the GPU; apps should use 
                // fences to determine GPU execution progress.
                ThrowIfFailed(mCommandAllocator[mFrameIndex]->Reset());

                // However, when ExecuteCommandList() is called on a particular command 
                // list, that command list can then be reset at any time and must be before 
                // re-recording.
                ThrowIfFailed(mCommandList->Reset(mCommandAllocator[mFrameIndex].Get(), mPipelineState.Get()));

                // Set necessary state.
                mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
                ID3D12DescriptorHeap* heaps[] = { mCBVSRVUAVHeap.Get() };
                mCommandList->SetDescriptorHeaps(1, heaps);
                mCommandList->RSSetViewports(1, &mViewport);
                mCommandList->RSSetScissorRects(1, &mScissorRect);

                // Indicate that the back buffer will be used as a render target.
                //mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
                PrepareResourceBarrierRenderTarget(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

                D3D12_CPU_DESCRIPTOR_HANDLE rtvHandler = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
                rtvHandler.ptr += mFrameIndex * mRtvDescriptorSize;
                D3D12_CPU_DESCRIPTOR_HANDLE dsvHandler = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
                mCommandList->OMSetRenderTargets(1, &rtvHandler, FALSE, &dsvHandler);

                // Record commands.
                float clearColor[4] = { 1.f, 1.f, 1.f, 1.0f };
                mCommandList->ClearRenderTargetView(rtvHandler, clearColor, 0, nullptr);
                mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                mCommandList->IASetVertexBuffers(0, 1, &mPosVertexBufferView);
                mCommandList->IASetVertexBuffers(1, 1, &mColorVertexBufferView);
                mCommandList->DrawInstanced(6, 1, 0, 0);
            }

            void Graphics_DX12::WaitForPreviousFrame()
            {
                // Wait until the previous frame is finished.
                if (mFence->GetCompletedValue() < mFenceValue[mFrameIndex])
                {
                    ThrowIfFailed(mFence->SetEventOnCompletion(mFenceValue[mFrameIndex], mFenceEvent));
                    WaitForSingleObject(mFenceEvent, INFINITE);
                }
            }


            void Graphics_DX12::CalculateWindowSize()
            {
                assert(Windows::GetWindowHandler() != 0);
                RECT clientRect;
                //.6f, .7f, .2f * windowWidth, 0
                GetClientRect(Windows::GetWindowHandler(), &clientRect);
                mClientWidth = (clientRect.right - clientRect.left);
                mClientHeight = (clientRect.bottom - clientRect.top);
            }

            // Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
            // If no such adapter can be found, *ppAdapter will be set to nullptr.
            _Use_decl_annotations_
                void Graphics_DX12::GetHardwareAdapter(
                    IDXGIFactory1* pFactory,
                    IDXGIAdapter1** ppAdapter,
                    bool requestHighPerformanceAdapter)
            {
                *ppAdapter = nullptr;

                ComPtr<IDXGIAdapter1> adapter;

                ComPtr<IDXGIFactory6> factory6;
                if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
                {
                    for (
                        UINT adapterIndex = 0;
                        SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                            adapterIndex,
                            requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                            IID_PPV_ARGS(&adapter)));
                            ++adapterIndex)
                    {
                        DXGI_ADAPTER_DESC1 desc;
                        adapter->GetDesc1(&desc);

                        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                        {
                            // Don't select the Basic Render Driver adapter.
                            // If you want a software adapter, pass in "/warp" on the command line.
                            continue;
                        }

                        // Check to see whether the adapter supports Direct3D 12, but don't create the
                        // actual device yet.
                        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                        {
                            break;
                        }
                    }
                }

                if (adapter.Get() == nullptr)
                {
                    for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
                    {
                        DXGI_ADAPTER_DESC1 desc;
                        adapter->GetDesc1(&desc);

                        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                        {
                            // Don't select the Basic Render Driver adapter.
                            // If you want a software adapter, pass in "/warp" on the command line.
                            continue;
                        }

                        // Check to see whether the adapter supports Direct3D 12, but don't create the
                        // actual device yet.
                        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                        {
                            break;
                        }
                    }
                }

                *ppAdapter = adapter.Detach();
            }


            ID3D12Device* Graphics_DX12::GetDevice()
            {
                return mDevice.Get();
            }


            UINT Graphics_DX12::GetFramesCount()
            {
                return FrameCount;
            }

            DXGI_FORMAT Graphics_DX12::GetRtvFormat()
            {
                return RTVFormat;
            }
            
            ID3D12DescriptorHeap* Graphics_DX12::GetCbvSrvHeap()
            {
                return mCBVSRVUAVHeap.Get();
            }

            D3D12_CPU_DESCRIPTOR_HANDLE Graphics_DX12::GetFontSrvCpuDescHandleForImGui()
            {
                D3D12_CPU_DESCRIPTOR_HANDLE cpu = mCBVSRVUAVHeap->GetCPUDescriptorHandleForHeapStart();
                // TODO: Use descriptor allocator to reserve a dedicated slot for ImGui
                // instead of assuming heap start. Avoid overwriting other CBV/SRV/UAV entries.
                //cpu.ptr += mImGuiDescriptorIndex * mDescriptorSize;
                return cpu;
            }

            D3D12_GPU_DESCRIPTOR_HANDLE Graphics_DX12::GetFontSrvGpuDescHandleForImGui()
            {
                D3D12_GPU_DESCRIPTOR_HANDLE gpu = mCBVSRVUAVHeap->GetGPUDescriptorHandleForHeapStart();
                // TODO: Use descriptor allocator to reserve a dedicated slot for ImGui
                // instead of assuming heap start. Avoid overwriting other CBV/SRV/UAV entries.
                // cpu.ptr += mImGuiDescriptorIndex * mDescriptorSize;
                return gpu;
            }

            ID3D12GraphicsCommandList* Graphics_DX12::GetCommandList()
            {
                return mCommandList.Get();
            }

        }
	}
}