#pragma once
//See about how to compile shader, to create a shader compiler in RUNTIME COMPILED SHADER session in the link below
//https://www.3dgep.com/introduction-to-directx-11/#Create_Device_and_Swap_Chain

#include "Windows.h"
#include "d3d11.h"
#include <cassert>
#include <vector>
#include "DrawableObject.h"
#include "Camera.h"
#include <wrl/client.h>
#include "Data/RColor.h"
#include "Data/RMatrix4x4.h"


// Shader resources
enum ConstantBuffer
{
	CB_Application,
	CB_Frame,
	CB_Object,
	CB_Globals,
	CB_Light,
	NumConstantBuffers
};

class GraphicsMain
{
public:
	static GraphicsMain* GetInstace()
	{
		return _instance;
	}

	static ID3D11Device* GetDevice() 
	{ 
		assert(_instance);
		return _instance->_device.Get(); 
	};

	static ID3D11DeviceContext* GetDeviceContext()
	{
		assert(_instance);
		return _instance->_deviceContext.Get();
	};

	static void UpdateConstantBuffer(ConstantBuffer type, const void* pSrcData)
	{
		_instance->_deviceContext->UpdateSubresource(_instance->_constantBuffers[type], 0, nullptr, pSrcData, 0, 0);
	}
	
	struct GlobalBuffer
	{
		int hasTexture;
		int boneSelectedId;
	};

	struct PerObjectBufer
	{
		RMatrix4x4 worldMatrix;
		RMatrix4x4 animTransformMatrix[MAX_NUM_OF_ANIMATION_CHANNELS];
	};

	float GetWidth() { return _clientWidth; }
	float GetHeight() { return _clientHeight; }

	GraphicsMain(HWND windowHandler);
	void SetupDevice();
	void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
	void Present(bool vSync);
	void Renderer();

	//TODO: new functions in shaderManager to upload constant buffers, this is just a test
	static int boneSelected;
	GlobalBuffer tempGlobalBuffer = {0, 0};
	static PerObjectBufer tempPerObjectBuffer;

private:
	static GraphicsMain* _instance;

	IDXGISwapChain* _swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	HWND _windowHandler;

	ID3D11DepthStencilState* _depthStencilState;
	D3D11_VIEWPORT _viewport = { 0 };
	ID3D11RasterizerState* _rasterizerState;
	ID3D11Buffer* _constantBuffers[NumConstantBuffers];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> _defaultSamplerState;

	float _clientWidth;
	float _clientHeight;
};