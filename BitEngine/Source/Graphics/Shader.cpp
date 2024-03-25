#include "Graphics/Shader.h"
#include "directxmath.h"
#include "d3dcompiler.h"
#include <stdexcept>
#include "Graphics/GraphicsMain.h"
#include "Faia/Memory.h"
#include "Faia/Converter.h"

using namespace DirectX;
using namespace Faia::Memory;

void Shader::Load(const char* shaderName)
{
    ID3D11Device* device = GraphicsMain::GetDevice();

    // Load the compiled vertex shader.
    ID3DBlob* vertexShaderBlob;
    std::string vertexShaderName(shaderName);
    vertexShaderName.append("VertexShader.cso");

    HRESULT hr = D3DReadFileToBlob(Faia::Converter::CharToLPCWSTR(vertexShaderName.c_str()), &vertexShaderBlob);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to read Vextex shader of name: ");
        msg.append((const char*)vertexShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create Vextex shader of name: ");
        msg.append((const char*)vertexShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    // Create the input layout for the vertex shader.
    D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), _inputLayout.GetAddressOf());
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create input shader of for vertex shader name: ");
        msg.append((const char*)vertexShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    SafeRelease(vertexShaderBlob);

    // Load the compiled pixel shader.
    ID3DBlob* pixelShaderBlob;
    std::string pixelShaderName(shaderName);
    pixelShaderName.append("PixelShader.cso");

    hr = D3DReadFileToBlob(Faia::Converter::CharToLPCWSTR(pixelShaderName.c_str()), &pixelShaderBlob);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to read file blob pixel shader of name: ");
        msg.append((const char*)pixelShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }

    hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create input pixel shader of name: ");
        msg.append((const char*)pixelShaderName.c_str());
        throw std::invalid_argument(msg.c_str());
    }


    SafeRelease(pixelShaderBlob);
    //LoadTexture(L"Content\\Textures\\Goat_Diffuse.png");
}
