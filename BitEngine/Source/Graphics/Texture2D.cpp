#include "Graphics/Texture2D.h"
#include "Graphics/GraphicsMain.h"
#include "Faia/Converter.h"
#include <string>
#include "Faia/Paths.h"
#include "DirectXTex.h"


using namespace Faia::Paths;


namespace Faia
{
    namespace Root
    {
        void Texture2D::Load(const char* textureRelativePath)
        {
            ID3D11Device* device = GraphicsMain::GetDevice();

            std::wstring texturePath(GetApplicationFolderPath());
            texturePath.append(Faia::Converter::CharToLPCWSTR(textureRelativePath));

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

            hr = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _textureSRV.GetAddressOf());


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

            hr = device->CreateSamplerState(&samplerDesc, _samplerState.GetAddressOf());
            if (FAILED(hr))
            {
                std::string msg;
                msg.append("Fail to create samplerState");
                OutputDebugStringA(msg.c_str());
                throw std::invalid_argument(msg);
            }
        }
    }
}