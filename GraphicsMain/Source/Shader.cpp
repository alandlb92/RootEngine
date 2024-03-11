#include "pch.h"
#include "Shader.h"
#include "directxmath.h"
#include <stdexcept>
#include "d3dcompiler.h"
#include "GraphicsMain.h"
#include "DirectXTex.h"

//TODO util module
std::wstring GetApplicationFolderPath() {
    WCHAR path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    std::wstring fullPath(path);

    // Encontrar a última barra invertida '\' no caminho do arquivo
    size_t lastSlashIndex = fullPath.find_last_of(L"\\");
    if (lastSlashIndex != std::wstring::npos) {
        // Extrair a pasta de aplicativo removendo o nome do arquivo
        return fullPath.substr(0, lastSlashIndex + 1);
    }

    // Se não for possível encontrar a barra invertida, retorna uma string vazia
    return L"";
}


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

void Shader::SetTextureResources(const wchar_t* texturePath)
{

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
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
    LoadTexture(L"Textures\\brick-wall-bl\\brick-wall_albedo.png");
}

void Shader::LoadTexture(const wchar_t* textureRelativePath)
{
    ID3D11Device* device = GraphicsMain::GetDevice();

    std::wstring texturePath(GetApplicationFolderPath());
    texturePath.append(textureRelativePath);

    ScratchImage image;
    HRESULT hr = LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_NONE, nullptr, image);

    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to load image path: ");
        msg.append((const char*)texturePath.c_str());
        OutputDebugStringA(msg.c_str());        
        throw std::invalid_argument(msg);
    }
    
    hr = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &_textureSRV);


    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc = D3D11_SAMPLER_DESC();
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.MinLOD = -FLT_MAX;
    samplerDesc.MaxLOD = FLT_MAX;

    hr = device->CreateSamplerState(&samplerDesc, &_samplerState);
    if (FAILED(hr))
    {
        std::string msg;
        msg.append("Fail to create samplerState");
        OutputDebugStringA(msg.c_str());
        throw std::invalid_argument(msg);
    }
}
