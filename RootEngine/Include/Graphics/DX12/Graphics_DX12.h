#pragma once

#include "d3d12.h"
#include "dxgi1_6.h"
#include "Graphics/RIGraphicsPlatform.h"

#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			class Graphics_DX12 : public RIGraphicsPlatform
			{
				public:
					void Setup() override;
					void BeginFrame() override;
					void EndFrame() override;
					ID3D12Device* GetDevice();
					UINT GetFramesCount();
					DXGI_FORMAT GetRtvFormat();
					ID3D12DescriptorHeap* GetCbvSrvHeap();
					D3D12_CPU_DESCRIPTOR_HANDLE GetFontSrvCpuDescHandleForImGui();
					D3D12_GPU_DESCRIPTOR_HANDLE GetFontSrvGpuDescHandleForImGui();
					ID3D12GraphicsCommandList* GetCommandList();

				private:
					void SetupDepthStencilView();
					void SetupViewportAndScissorRects();
					void SetupRootSignature();
					void SetupMainDescriptorHeap();
					void PrepareResourceBarrierRenderTarget(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
					void LoadShader(LPCWSTR FileName, D3D12_SHADER_BYTECODE& ShaderByteCode);
					void LoadAssets();
					void PopulateCommandList();
					void WaitForPreviousFrame();

					static const UINT FrameCount = 2;
					static const DXGI_FORMAT DephtStencilFormat = DXGI_FORMAT_D32_FLOAT;
					static const DXGI_FORMAT RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

					int mClientWidth;
					int mClientHeight;			
					void CalculateWindowSize();
					
					D3D12_VIEWPORT mViewport;
					D3D12_RECT mScissorRect;

					// Pipeline objects.
					ComPtr<ID3D12Device> mDevice;
					ComPtr<IDXGISwapChain3> mSwapChain;
					ComPtr<ID3D12Resource> mRenderTargets[FrameCount];
					ComPtr<ID3D12Resource> mDepthStencilBuffer;
					ComPtr<ID3D12CommandAllocator> mCommandAllocator[FrameCount];
					ComPtr<ID3D12CommandQueue> mCommandQueue;
					ComPtr<ID3D12RootSignature> mRootSignature;
					ComPtr<ID3D12DescriptorHeap> mRtvHeap;
					ComPtr<ID3D12DescriptorHeap> mDsvHeap;
					ComPtr<ID3D12DescriptorHeap> mCBVSRVUAVHeap;
					ComPtr<ID3D12PipelineState> mPipelineState;
					ComPtr<ID3D12GraphicsCommandList> mCommandList;
					UINT mRtvDescriptorSize;
					D3D12_VERTEX_BUFFER_VIEW mPosVertexBufferView;
					D3D12_VERTEX_BUFFER_VIEW mColorVertexBufferView;

					// Synchronization objects.
					UINT64 mCurrentFenceValue = 1;
					UINT mFrameIndex;
					HANDLE mFenceEvent;
					ComPtr<ID3D12Fence> mFence;
					UINT64 mFenceValue[FrameCount] = {0, 0};

					// Adapter info.
					bool mUseWarpDevice;


					void GetHardwareAdapter(
						_In_ IDXGIFactory1* pFactory,
						_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
						bool requestHighPerformanceAdapter = false);
			};

			using GraphicsPlatform = Graphics_DX12;
		}
	}
}