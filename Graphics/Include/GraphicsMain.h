#pragma once
//See about how to compile shader, to create a shader compiler in RUNTIME COMPILED SHADER session in the link below
//https://www.3dgep.com/introduction-to-directx-11/#Create_Device_and_Swap_Chain

#include "Windows.h"
#include "d3d11.h"
#include <cassert>
#include <vector>
#include "DrawableObject.h"
#include "Camera.h"

// Shader resources
enum ConstantBuffer
{
	CB_Application,
	CB_Frame,
	CB_Object,
	NumConstantBuffers
};

class GraphicsMain
{
public:
	static ID3D11Device* GetDevice() 
	{ 
		assert(_instance);
		return _instance->_device; 
	};

	static ID3D11DeviceContext* GetDeviceContext()
	{
		assert(_instance);
		return _instance->_deviceContext;
	};

	static void UpdateConstantBuffer(ConstantBuffer type, const void* pSrcData)
	{
		_instance->_deviceContext->UpdateSubresource(_instance->_constantBuffers[type], 0, nullptr, pSrcData, 0, 0);
	}

	GraphicsMain(HWND windowHandler);
	void Init();
	void Update(float deltaTime);
	void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
	void Present(bool vSync);
	void Renderer();

private:
	Camera _camera;
	std::vector<DrawableObject> _drawableObjects;
	

	static GraphicsMain* _instance;

	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	HWND _windowHandler;

	ID3D11DepthStencilState* _depthStencilState;
	D3D11_VIEWPORT _viewport = { 0 };
	ID3D11RasterizerState* _rasterizerState;
	ID3D11Buffer* _constantBuffers[NumConstantBuffers];


	bool LoadContent();
};