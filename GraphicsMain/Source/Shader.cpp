#include "pch.h"
#include "Shader.h"
#include "directxmath.h"
#include <stdexcept>
#include "d3dcompiler.h"
#include "GraphicsMain.h"


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

using namespace DirectX;

Shader Shader::MakeSimpleShader()
{
    Shader shader;
    shader._shaderName = L"Simple";
    shader.Load();

    return shader;
}

Shader::Shader()
{
}

Shader::Shader(const wchar_t* shaderName)
{
    _shaderName = shaderName;
    Load();
}

void Shader::Load()
{
    ID3D11Device* device = GraphicsMain::GetDevice();

    // Load the compiled vertex shader.
    ID3DBlob* vertexShaderBlob;
    std::wstring vertexShaderName(_shaderName);
    vertexShaderName.append(L"VertexShader.cso");

    HRESULT hr = D3DReadFileToBlob(vertexShaderName.c_str(), &vertexShaderBlob);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to read Vextex shader of name: ");
        msg.append((const char*) vertexShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &_vertexShader);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create Vextex shader of name: ");
        msg.append((const char*) vertexShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    // Create the input layout for the vertex shader.
    D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &_inputLayout);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create input shader of for vertex shader name: ");
        msg.append((const char*) vertexShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    SafeRelease(vertexShaderBlob);

    // Load the compiled pixel shader.
    ID3DBlob* pixelShaderBlob;
    std::wstring pixelShaderName(_shaderName);
    pixelShaderName.append(L"PixelShader.cso");

    hr = D3DReadFileToBlob(pixelShaderName.c_str(), &pixelShaderBlob);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to read file blob pixel shader of name: ");
        msg.append((const char*) pixelShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &_pixelShader);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create input pixel shader of name: ");
        msg.append((const char*)pixelShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    SafeRelease(pixelShaderBlob);

}
