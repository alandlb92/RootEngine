// GraphicsMain.cpp : Defines the functions for the static library.
//

#include "Graphics/GraphicsMain.h"
#include "directxmath.h"
#include <DirectXColors.h>
#include "Core/Scene/SceneManager.h";
#include <numbers>
#include "Graphics/Mesh/RSkeletalMesh.h"
#include "Data/RMeshData.h"
#include "Components/RAnimationComponent.h"
#include "Faia/WindowsApplication.h"
#include "Graphics/RConstantBuffersHandler.h"
#include "Faia/HashUtils.h"
#include "Graphics/LightManager.h"

using namespace DirectX;
using namespace Faia::Root;

namespace Faia
{
    namespace Root
    {
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


        GraphicsMain* gGraphics;
        GraphicsMain* GetGraphics()
        {
            if (gGraphics == nullptr)
            {
                gGraphics = new GraphicsMain();
            }

            return gGraphics;
        }

        ID3D11Device* GetDevice()
        {
            assert(GetGraphics());
            return GetGraphics()->_device.Get();
        };

        ID3D11DeviceContext* GetDeviceContext()
        {
            assert(GetGraphics());
            return GetGraphics()->_deviceContext.Get();
        };

        GraphicsMain::GraphicsMain()
        {
        }

        void GraphicsMain::SetupDevice()
        {
            // A window handle must have been created already.
            assert(Windows::GetWindowHandler() != 0);
            RECT clientRect;
            GetClientRect(Windows::GetWindowHandler(), &clientRect);

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
            swapChainDesc.OutputWindow = Windows::GetWindowHandler();
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

            // Compute the exact client dimensions.
            // This is required for a correct projection matrix.
            _clientWidth = static_cast<float>(clientRect.right - clientRect.left);
            _clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

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

            hr = _device->CreateSamplerState(&samplerDesc, _defaultSamplerState.GetAddressOf());
            if (FAILED(hr))
            {
                std::string msg;
                msg.append("Fail to create samplerState");
                OutputDebugStringA(msg.c_str());
            }
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

            Graphics::GetConstantBuffersHandler()->UpdateSubresource(Graphics::gPerApplicationHash);
            Graphics::GetConstantBuffersHandler()->UpdateSubresource(Graphics::gPerFrameHash);
            Graphics::Light::LightManager::GetInstance()->UpdateLightToCB();

            //We can put some parts of the code in other classes, for exemple the mesh upload data can be inside RMeshRenderer or something like that
            for (auto& ro : GetSceneManager()->GetCurrentScene()->GetRenderablebleObjects())
            {
                //Todo: avoid GetComponentOfType do it when create RenderData
                RAnimationComponent* anim = ro->GetComponentOfType<RAnimationComponent>();
                if (anim)
                {
                    Faia::Root::RMatrix4x4 animMatrix[MAX_NUM_OF_ANIMATION_CHANNELS];
                    anim->GetAnimationChannelsMatrix(animMatrix);
                    Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gAnimMatrixHash, &animMatrix);
                }

                uint32_t skinned = anim ? 1 : 0;
                Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gIsSkinnedHash, &skinned);


                for (std::shared_ptr<RMesh> mesh : ro->GetMeshComponent()->GetMeshs())
                {
                    uint32_t indexCount = 0;
                    int materialIndex = mesh->GetMaterialIndex();
                    RMaterial* material = ro->GetMaterialComponent()->GetMaterialOfIndex(materialIndex);

                    XMFLOAT3 XMFObjPos = XMFLOAT3(ro->GetPosition().X, ro->GetPosition().Y, ro->GetPosition().Z);
                    XMVECTOR XMVObjPos = XMLoadFloat3(&XMFObjPos);

                    const float pi = 3.14159265358979323846f;
                    RVector3D radianVector = ro->GetRotation() * (pi / 180);
                    XMFLOAT3 XMFObjRot = XMFLOAT3(radianVector.X, radianVector.Y, radianVector.Z);
                    XMVECTOR XMVObjRot = XMLoadFloat3(&XMFObjRot);


                    XMFLOAT3 XMFObjSca = XMFLOAT3(ro->GetScale().X, ro->GetScale().Y, ro->GetScale().Z);
                    XMVECTOR XMVObjSca = XMLoadFloat3(&XMFObjSca);


                    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(XMVObjPos);
                    XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYawFromVector(XMVObjRot);
                    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
                    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(XMVObjSca);

                    DirectX::XMMATRIX transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

                    Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gWorldMatrixHash, &transformMatrix);

                    _deviceContext->IASetInputLayout(material->GetShader()->GetInputLayout());

                    const UINT vertexStride = sizeof(RVector3D);
                    const UINT uvStride = sizeof(RVector2D);
                    const UINT normalStride = sizeof(RVector3D);
                    UINT offset = 0;

                    ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
                    ID3D11Buffer* uvBuffer = mesh->GetUvBuffer();
                    ID3D11Buffer* normalBuffer = mesh->GetNormalBuffer();
                    ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();

                    _deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &offset);
                    _deviceContext->IASetVertexBuffers(1, 1, &uvBuffer, &uvStride, &offset);
                    _deviceContext->IASetVertexBuffers(2, 1, &normalBuffer, &normalStride, &offset);

                    if(skinned)
                    {
                        std::shared_ptr<RSkeletalMesh> sMesh = std::dynamic_pointer_cast<RSkeletalMesh>(mesh);
                        const UINT boneDataStride = sizeof(RVertexBoneData);
                        ID3D11Buffer* boneDataBuffer = sMesh->GetBonesDataBuffer();
                        if (boneDataBuffer)
                        {
                            if (_deviceContext)
                            {
                                _deviceContext->IASetVertexBuffers(3, 1, &boneDataBuffer, &boneDataStride, &offset);
                            }
                        }
                    }

                    _deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

                    indexCount += mesh->GetIndicesSize();
                    _deviceContext->VSSetShader(material->GetShader()->GetVertexShader(), nullptr, 0);
                    _deviceContext->PSSetShader(material->GetShader()->GetPixelShader(), nullptr, 0);

                    ID3D11ShaderResourceView* textureSRV = nullptr;
                    ID3D11SamplerState* samplerState = _defaultSamplerState.Get();
                    if (material->GetTexture(0))
                    {
                        textureSRV = material->GetTexture(0)->GetTexture();
                        samplerState = material->GetTexture(0)->GetSamplerState();
                    }

                    uint32_t hasTexture = textureSRV == NULL ? 0 : 1;
                    Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gHasTextureHash, &hasTexture);

                    Graphics::GetConstantBuffersHandler()->UpdateSubresource(Graphics::gPerObjectHash);

                    _deviceContext->PSSetShaderResources(0, 1, &textureSRV);
                    _deviceContext->PSSetSamplers(0, 1, &samplerState);
                    Graphics::GetConstantBuffersHandler()->PSSetConstantBuffers();

                    _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    Graphics::GetConstantBuffersHandler()->VSSetConstantBuffers();
                    _deviceContext->RSSetState(_rasterizerState);
                    _deviceContext->RSSetViewports(1, &_viewport);

                    _deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
                    _deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

                    _deviceContext->DrawIndexed(indexCount, 0, 0);
                }
            }
            //TODO: i cant use vsync right now because this will bring a problem in input system
            Present(false);
        }
    }
}