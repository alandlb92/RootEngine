// GraphicsMain.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "GraphicsMain.h"
#include "directxmath.h"
#include <DirectXColors.h>

using namespace DirectX;


// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr)
{
    if (ptr != NULL)
    {
        ptr->Release();
        ptr = NULL;
    }
}


GraphicsMain* GraphicsMain::_instance = nullptr;

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
        D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, _device.GetAddressOf(), &featureLevel,
        _deviceContext.GetAddressOf());

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
    hr = _device->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create render target view\n");
    }

    SafeRelease(backBuffer);

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

    hr = _device->CreateDepthStencilView(depthStencilBuffer, nullptr, &_depthStencilView);
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

    hr = _device->CreateDepthStencilState(&depthStencilStateDesc, &_depthStencilState);
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

    // Create the rasterizer state object.
    hr = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
    if (FAILED(hr))
    {
        OutputDebugStringA("Failed to create rasterizer state\n");
    }

    _viewport.Width = static_cast<float>(clientWidth);
    _viewport.Height = static_cast<float>(clientHeight);
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;

    _instance = this;

    LoadContent();
}

void GraphicsMain::Update(float deltaTime)
{
    _camera->Update(deltaTime);
}

void GraphicsMain::Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil)
{
    _deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);
    if (_depthStencilView)
        _deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

void GraphicsMain::Present(bool vSync)
{
    if (vSync)
    {
        _swapChain->Present(1, 0);
    }
    else
    {
        _swapChain->Present(0, 0);
    }
}

void GraphicsMain::Renderer()
{
    assert(_device);
    assert(_deviceContext);

    Clear(Colors::CornflowerBlue, 1.0f, 0);


    uint32_t indexCount = 0;

    for (auto& d : _drawableObjects)
    {
        for (auto& mesh : d.GetMeshs())
        {
            int materialIndex = mesh.GetMaterialIndex();
            Material material = d.GetMaterial(mesh.GetMaterialIndex());

            _deviceContext->IASetInputLayout(material.GetShader()->GetInputLayout());

            const UINT vertexStride = sizeof(Vector3D);
            const UINT uvStride = sizeof(Vector2D);
            const UINT offset = 0;

            ID3D11Buffer* vertexBuffer = mesh.GetVertexBuffer();
            ID3D11Buffer* uvBuffer = mesh.GetUvBuffer();
            ID3D11Buffer* indexBuffer = mesh.GetIndexBuffer();

            _deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &offset);
            _deviceContext->IASetVertexBuffers(1, 1, &uvBuffer, &uvStride, &offset);
            _deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);


            indexCount += mesh.GetIndicesSize();
            _deviceContext->VSSetShader(material.GetShader()->GetVertexShader(), nullptr, 0);
            _deviceContext->PSSetShader(material.GetShader()->GetPixelShader(), nullptr, 0);

            ID3D11ShaderResourceView* textureSRV = material.GetTexture(0)->GetTexture();
            ID3D11SamplerState* samplerState = material.GetTexture(0)->GetSamplerState();
            _deviceContext->PSSetShaderResources(0, 1, &textureSRV);
            _deviceContext->PSSetSamplers(0, 1, &samplerState);

            _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            _deviceContext->VSSetConstantBuffers(0, 3, _constantBuffers);
            _deviceContext->RSSetState(_rasterizerState);
            _deviceContext->RSSetViewports(1, &_viewport);

            _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
            _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

            _deviceContext->DrawIndexed(indexCount, 0, 0);

        }
    }

    Present(false);
}



bool GraphicsMain::LoadContent()
{
    assert(_device);

    // Setup the projection matrix.
    RECT clientRect;
    GetClientRect(_windowHandler, &clientRect);

    // Compute the exact client dimensions.
    // This is required for a correct projection matrix.
    _clientWidth = static_cast<float>(clientRect.right - clientRect.left);
    _clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

    //std::vector<Mesh> meshs = { Mesh::MakePrimitiveCube() };

    std::vector<Mesh> meshs = Mesh::MakeFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\HeroGoat.bitMesh");

    Material material0;
    material0.SetShader("Simple");
    material0.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1001_Diffuse.png", 0);

    Material material1;
    material1.SetShader("Simple");
    material1.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1002_Diffuse.png", 0);

    Material material2;
    material2.SetShader("Simple");
    material2.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1002_Diffuse.png", 0);

    Material material3;
    material3.SetShader("Simple");
    material3.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1003_Diffuse.png", 0);

    _drawableObjects = { DrawableObject(meshs, { material0, material1, material2, material3 }) };

    // Create the constant buffers for the variables defined in the vertex shader.
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    HRESULT hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Application]);
    if (FAILED(hr))
    {
        return false;
    }
    hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Frame]);
    if (FAILED(hr))
    {
        return false;
    }
    hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffers[CB_Object]);
    if (FAILED(hr))
    {
        return false;
    }

    _camera = std::make_unique<Camera>();
    _camera->Init();
    
    return true;
}
