// GraphicsMain.cpp : Defines the functions for the static library.
//

#include "pch.h"

#include "pch.h"
#include "GraphicsMain.h"
#include <cassert>

GraphicsMain::GraphicsMain(HWND windowHandler)
{
    _windowHandler = windowHandler;
}

void GraphicsMain::Init()
{
    // A window handle must have been created already.
    assert(_windowHandler != 0);
    RECT clientRect;
    GetClientRect(_windowHandler, &clientRect);

    // Compute the exact client dimensions. This will be used
    // to initialize the render targets for our swap chain.
    unsigned int clientWidth = clientRect.right - clientRect.left;
    unsigned int clientHeight = clientRect.bottom - clientRect.top;

    //Swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = clientWidth;
    swapChainDesc.BufferDesc.Height = clientHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = _windowHandler;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed = TRUE;

    UINT createDeviceFlags = 0;
#if _DEBUG
    createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };


    // is used to create our device and swap chain.
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
        D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, &featureLevel,
        &_deviceContext);

    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create device and swap chain\n");
    }

    //Create backbuffer
    ID3D11Texture2D* backBuffer;
    hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to get back buffer\n");
    }

    //create render target view
    ID3D11RenderTargetView* _renderTargetView;
    hr = _device->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create render target view\n");
    }

    //Depht stencil description
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = clientWidth;
    depthStencilBufferDesc.Height = clientHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Texture2D* depthStencilBuffer;
    //Create depht stencil texture
    hr = _device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &depthStencilBuffer);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create depht stencil texture\n");
    }

    ID3D11DepthStencilView* depthStencilView;
    hr = _device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create depht stencil view\n");
    }

    // Setup depth/stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = FALSE;

    ID3D11DepthStencilState* depthStencilState;
    hr = _device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to Create depth stencil state\n");
    }

    // Setup rasterizer state.
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    ID3D11RasterizerState* rasterizerState;
    // Create the rasterizer state object.
    hr = _device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create rasterizer state\n");
    }

    D3D11_VIEWPORT viewport = { 0 };
    viewport.Width = static_cast<float>(clientWidth);
    viewport.Height = static_cast<float>(clientHeight);
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

}


void GraphicsMain::Run()
{
}