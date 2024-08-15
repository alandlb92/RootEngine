#include "Graphics/Texture2D.h"
#include "Graphics/GraphicsMain.h"
#include "Faia/Converter.h"
#include <string>
#include "Faia/Paths.h"
//todo: remove directXText to more mult platform way
//#include "DirectXTex.h"


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

            //Texture description
            D3D11_TEXTURE2D_DESC texDesc;
            ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
            texDesc.Width = 5;
            texDesc.Height = 5;
            texDesc.MipLevels = 1;
            texDesc.ArraySize = 1;
            texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            texDesc.SampleDesc.Count = 1;
            texDesc.SampleDesc.Quality = 0;
            texDesc.Usage = D3D11_USAGE_DEFAULT;
            texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            texDesc.CPUAccessFlags = 0;
            texDesc.MiscFlags =  0;
            //Todo: read the texture from a file we will do a importer for the texture.
            struct rgbaTest
            {
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
            };


            rgbaTest tex[25] = {rgbaTest{255, 0, 0, 255}, rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},
                                rgbaTest{255, 0, 0, 255}, rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},
                                rgbaTest{255, 0, 0, 255}, rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},
                                rgbaTest{255, 0, 0, 255}, rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},
                                rgbaTest{255, 0, 0, 255}, rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255},rgbaTest{255, 0, 0, 255}
                                };

            //Texture data
            D3D11_SUBRESOURCE_DATA texData;
            ZeroMemory(&texData, sizeof(D3D11_SUBRESOURCE_DATA));
            texData.pSysMem = tex;
            texData.SysMemPitch = texDesc.Width * sizeof(rgbaTest);
            texData.SysMemSlicePitch = 0;

            //ScratchImage image;
            ID3D11Texture2D* texture = nullptr;
            HRESULT hr = device->CreateTexture2D(&texDesc, &texData, &texture);

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
            srvDesc.Format = texDesc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = -1;


            hr = device->CreateShaderResourceView(texture, &srvDesc, _textureSRV.GetAddressOf());

            //Sampler

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