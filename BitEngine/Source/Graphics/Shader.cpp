#include "Graphics/Shader.h"
#include "directxmath.h"
#include "d3dcompiler.h"
#include <stdexcept>
#include "Graphics/GraphicsMain.h"
#include "Faia/Memory.h"
#include "Faia/Converter.h"
#include "Data/BitMeshData.h"

using namespace DirectX;
using namespace Faia::Memory;

void Shader::Load(const char* shaderName)
{
    _shaderName = std::string(shaderName);

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

    D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc;
    //TODO: This is just temp for tests
    if (shaderName == "SimpleSkeleton")
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutDesc;
        vertexLayoutDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        vertexLayoutDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        vertexLayoutDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        
        UINT offset = 0;
        for (UINT i = 0; i < MAX_NUM_OF_BONES_PER_VERTEX; ++i)
        {
            vertexLayoutDesc.push_back({ "BONEDATA", i, DXGI_FORMAT_R32_SINT, 3, offset, D3D11_INPUT_PER_VERTEX_DATA, 0});
            offset += sizeof(uint32_t);
        }

        for (UINT i = 0; i < MAX_NUM_OF_BONES_PER_VERTEX; ++i)
        {
            vertexLayoutDesc.push_back({ "BONEDATA", (UINT)MAX_NUM_OF_BONES_PER_VERTEX + i, DXGI_FORMAT_R32_FLOAT, 3,  offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
            offset += sizeof(float);
        }

        hr = device->CreateInputLayout(vertexLayoutDesc.data(), vertexLayoutDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), _inputLayout.GetAddressOf());

    }
    else
    { 
        // Create the input layout for the vertex shader.
        D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), _inputLayout.GetAddressOf());
    }
    

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
