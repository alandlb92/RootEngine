#pragma once
//See about how to compile shader, to create a shader compiler in RUNTIME COMPILED SHADER session in the link below
//https://www.3dgep.com/introduction-to-directx-11/#Create_Device_and_Swap_Chain

#include "RCamera.h"
#include "Data/RColor.h"
#include "Data/RMatrix4x4.h"

#include "Windows.h"
#include "d3d11.h"

#include <cassert>
#include <vector>


#include <functional>


namespace Faia
{
	namespace Root
	{

		using PostRenderFunction = std::function<void()>;

		class GraphicsMain;
		GraphicsMain* GetGraphics();
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11ShaderResourceView* GetRenderOutSRV();
		void ResizeViewport(float width, float height);

		class GraphicsMain
		{
			friend ID3D11Device* GetDevice();
			friend ID3D11DeviceContext* GetDeviceContext();
			friend ID3D11ShaderResourceView* GetRenderOutSRV();
			friend void ResizeViewport(float width, float height);

		public:
			float GetWidth() { return _clientWidth; }
			float GetHeight() { return _clientHeight; }

			GraphicsMain();
			void SetupDevice();
			void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
			void Present(bool vSync);
			void Renderer();

			void RegisterPostRendererFunction(PostRenderFunction postRendererFunction);

		private:
			std::vector<PostRenderFunction> mPostRenderFunctions;

			IDXGISwapChain* _swapChain;
			Microsoft::WRL::ComPtr<ID3D11Device> _device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

			ID3D11RenderTargetView* _renderTargetView;
			ID3D11DepthStencilView* _depthStencilView = nullptr;

			ID3D11DepthStencilState* _depthStencilState;
			D3D11_VIEWPORT mViewport = { 0 };
			ID3D11RasterizerState* _rasterizerState;

			Microsoft::WRL::ComPtr<ID3D11SamplerState> _defaultSamplerState;

			float _clientWidth;
			float _clientHeight;
			//Resize need to change _viewport, renderTargetDesc and camera aspect ratio
			void ConfigureViewport(float width, float height);
			void ConfigureDepthStencilView(float width, float height);

#if defined _EDITOR
			void ConfigureOutSrv(float width, float height);
			ID3D11ShaderResourceView* mRenderOutSRV;
			ID3D11RenderTargetView* mRenderOutTargetView;
#endif

		};
	}
}