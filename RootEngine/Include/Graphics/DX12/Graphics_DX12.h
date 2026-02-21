#pragma once

#include "d3d12.h"
#include "dxgi.h"

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			constexpr int SwapChainBufferCount = 2;
			constexpr DXGI_FORMAT DephtStencilFormat = DXGI_FORMAT_D32_FLOAT;
			constexpr DXGI_FORMAT SwapChainAndRTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			class Graphics_DX12
			{
				public:
					void SetupDevice();
					ID3D12Device* GetDevice();					
					DXGI_FORMAT GetRtvFormat();
					ID3D12DescriptorHeap* GetCbvSrvHeap();
					D3D12_CPU_DESCRIPTOR_HANDLE GetFontSrvCpuDescHandleForImGui();
					D3D12_GPU_DESCRIPTOR_HANDLE GetFontSrvGpuDescHandleForImGui();
					ID3D12GraphicsCommandList* GetCommandList();

				private:
					int mClientWidth;
					int mClientHeight;

					ID3D12Device* mDevice;					
					ID3D12Fence* mFence;
					ID3D12CommandQueue* mCommandQueue;
					ID3D12CommandAllocator* mCommandAllocator;
					ID3D12GraphicsCommandList* mCommandList;
					IDXGISwapChain* mSwapChain;

					ID3D12DescriptorHeap* mRTVHeap;
					ID3D12DescriptorHeap* mDSVHeap;
					ID3D12DescriptorHeap* mCBVSRVUAVHeap;
					ID3D12Resource* mSwapChainBuffer[SwapChainBufferCount];
					ID3D12Resource* mDepthStencilBuffer;
					ID3D12RootSignature* mRootSignature;
					D3D12_RECT mScissorRect;
					D3D12_SHADER_BYTECODE mShaderByteCodeVS;
					D3D12_INPUT_ELEMENT_DESC mInputLayout[2];
					D3D12_INPUT_LAYOUT_DESC mInputLayoutDesc;
					D3D12_VERTEX_BUFFER_VIEW mPosVertexBufferView;
					D3D12_VERTEX_BUFFER_VIEW mColorVertexBufferView;
					D3D12_SHADER_BYTECODE mShaderByteCodePS;
					ID3D12PipelineState* mPipelineState;
					
					void CalculateWindowSize();
					void CreateDeviceAndFence();
					void CreateCommandQueueAllocatorAndList();
					void CreateDXGIFactoryAndSwapChain();
					void CreateRenderTargetView();
					void CreateDepthStencilView();
					void CreateMainDescriptorHeap();
					void CreateAndSetRootSignature(); 
					void SetViewportAndScissorRects(); 
					void ResourceBarrierRenderTarget(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
					void Clear();

					void SetRenderTargets();
					void SetupInputLayout();
					void LoadVS();
					void SetVertexBuffer();
					void LoadPS();
					void CreateAndSetPipelineState();

					void CommitAndPresent();
			};

			Graphics_DX12* GetGraphics();
		}
	}
}