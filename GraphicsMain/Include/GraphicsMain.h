#pragma once
#include "Windows.h"
#include "d3d11.h"

class GraphicsMain
{
public:
	GraphicsMain(HWND windowHandler);
	void Init();
	void Run();

private:
	HWND hwnd;
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	HWND _windowHandler;
};