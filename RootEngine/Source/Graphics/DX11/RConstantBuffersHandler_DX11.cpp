#include "Graphics/DX11/RConstantBuffersHandler_DX11.h"
#include "Graphics/DX11/GraphicsMain_DX11.h"
#include "d3d11.h"
#include "Faia/Debug.h"
#include "Data/RColor.h"
#include "Graphics/DX11/LightManager_DX11.h"

namespace Faia
{
    namespace Root
    {
        namespace Graphics
        {
            uint32_t gPerApplicationHash;
            uint32_t gProjectionMatrixHash;

            uint32_t gPerFrameHash;
            uint32_t gViewMatrixHash;
            uint32_t gcameraPositionHash;
            
            uint32_t gPerObjectHash;
            uint32_t gIsSkinnedHash;
            uint32_t gWorldMatrixHash;
            uint32_t gAnimMatrixHash;
            uint32_t gHasTextureHash;
            
            uint32_t gLightBufferHash;
            uint32_t gLightData;

            void InitializeGHashs()
            {
                gPerApplicationHash = HashUtils::CharToHashFnv1a("PerApplication");
                gProjectionMatrixHash = HashUtils::CharToHashFnv1a("ProjectionMatrix");
                gPerFrameHash = HashUtils::CharToHashFnv1a("PerFrame");
                gViewMatrixHash = HashUtils::CharToHashFnv1a("ViewMatrix");
                gcameraPositionHash = HashUtils::CharToHashFnv1a("CameraPosition");
                gPerObjectHash = HashUtils::CharToHashFnv1a("PerObject");
                gIsSkinnedHash = HashUtils::CharToHashFnv1a("IsSkinned");
                gWorldMatrixHash = HashUtils::CharToHashFnv1a("WorldMatrix");
                gAnimMatrixHash = HashUtils::CharToHashFnv1a("AnimMatrix");
                gHasTextureHash = HashUtils::CharToHashFnv1a("HasTexture");
                gLightBufferHash = HashUtils::CharToHashFnv1a("LightBuffer");
                gLightData = HashUtils::CharToHashFnv1a("LightData");
            }

            RConstantBuffersHandler_DX11* gConstantBuffersHandler;
            RConstantBuffersHandler_DX11* GetConstantBuffersHandler()
            {
                if (gConstantBuffersHandler == nullptr)
                {
                    gConstantBuffersHandler = new RConstantBuffersHandler_DX11();
                    gConstantBuffersHandler->Initialize();
                }

                return gConstantBuffersHandler;
            }

            void RConstantBuffersHandler_DX11::Initialize()
            {
                InitializeGHashs();

                //todo: Read cbInfo first
                //todo: remove and read, just populating cbInfo for test propouses
                //Per App
                CB_Info cbInfoPerApplication;
                cbInfoPerApplication.mHashName = HashUtils::CharToHashFnv1a("PerApplication");
                cbInfoPerApplication.mSlot = 0;
                cbInfoPerApplication.mSize = 64;
                CB_Param param1PerApp;
                param1PerApp.mHashName = HashUtils::CharToHashFnv1a("ProjectionMatrix");
                param1PerApp.mSize = 64;
                RMatrix4x4 matrixTest = RMatrix4x4::Identity();
                param1PerApp.mDefaultParams = std::vector<uint8_t>(param1PerApp.mSize);
                std::memcpy(param1PerApp.mDefaultParams.data(), &matrixTest, param1PerApp.mSize);
                cbInfoPerApplication.mParams.push_back(param1PerApp);

                //Per frame
                CB_Info cbInfoPerFrame;
                cbInfoPerFrame.mHashName = HashUtils::CharToHashFnv1a("PerFrame");
                cbInfoPerFrame.mSlot = 1;
                cbInfoPerFrame.mSize = 64 + 12;
                CB_Param param1PerFrame;
                param1PerFrame.mHashName = HashUtils::CharToHashFnv1a("ViewMatrix");
                param1PerFrame.mSize = 64;
                param1PerFrame.mDefaultParams = std::vector<uint8_t>(param1PerFrame.mSize);
                std::memcpy(param1PerFrame.mDefaultParams.data(), &matrixTest, param1PerFrame.mSize);
                cbInfoPerFrame.mParams.push_back(param1PerFrame);
                CB_Param param2PerFrame;
                param2PerFrame.mHashName = HashUtils::CharToHashFnv1a("CameraPosition");
                param2PerFrame.mSize = 12;
                param2PerFrame.mDefaultParams = std::vector<uint8_t>(param2PerFrame.mSize);
                RVector3D vectorDefaultValue(22);
                std::memcpy(param2PerFrame.mDefaultParams.data(), &vectorDefaultValue, param2PerFrame.mSize);
                cbInfoPerFrame.mParams.push_back(param2PerFrame);

                //Per Object
                CB_Info cbInfoPerObject;
                cbInfoPerObject.mHashName = HashUtils::CharToHashFnv1a("PerObject");
                cbInfoPerObject.mSlot = 2;
                cbInfoPerObject.mSize = 16 + 64 + (64 * MAX_NUM_OF_ANIMATION_CHANNELS);

                CB_Param param1PerObject;
                param1PerObject.mHashName = HashUtils::CharToHashFnv1a("HasTexture");
                param1PerObject.mSize = 4;
                uint32_t intTest = 0;
                param1PerObject.mDefaultParams = std::vector<uint8_t>(param1PerObject.mSize);
                std::memcpy(param1PerObject.mDefaultParams.data(), &intTest, param1PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param1PerObject);                

                CB_Param param2PerObject;
                param2PerObject.mHashName = HashUtils::CharToHashFnv1a("IsSkinned");
                param2PerObject.mSize = 4;
                param2PerObject.mDefaultParams = std::vector<uint8_t>(param2PerObject.mSize);
                std::memcpy(param2PerObject.mDefaultParams.data(), &intTest, param2PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param2PerObject);                
                

                CB_Param param3PerObject;
                param3PerObject.mHashName = HashUtils::CharToHashFnv1a("offset");
                param3PerObject.mSize = 4;
                param3PerObject.mDefaultParams = std::vector<uint8_t>(param3PerObject.mSize);
                std::memcpy(param3PerObject.mDefaultParams.data(), &intTest, param3PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param3PerObject);


                CB_Param param4PerObject;
                param4PerObject.mHashName = HashUtils::CharToHashFnv1a("offset");
                param4PerObject.mSize = 4;
                param4PerObject.mDefaultParams = std::vector<uint8_t>(param4PerObject.mSize);
                std::memcpy(param4PerObject.mDefaultParams.data(), &intTest, param4PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param2PerObject);

                CB_Param param5PerObject;
                param5PerObject.mHashName = HashUtils::CharToHashFnv1a("WorldMatrix");
                param5PerObject.mSize = 64;
                param5PerObject.mDefaultParams = std::vector<uint8_t>(param5PerObject.mSize);
                std::memcpy(param5PerObject.mDefaultParams.data(), &matrixTest, param5PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param5PerObject);

                CB_Param param6PerObject;
                param6PerObject.mHashName = HashUtils::CharToHashFnv1a("AnimMatrix");
                param6PerObject.mSize = 64 * MAX_NUM_OF_ANIMATION_CHANNELS;

                RMatrix4x4 matrixTestArr[MAX_NUM_OF_ANIMATION_CHANNELS];
                for (int i = 0; i < MAX_NUM_OF_ANIMATION_CHANNELS; ++i)
                {
                    matrixTestArr[i] = RMatrix4x4::Identity();
                }
                param6PerObject.mDefaultParams = std::vector<uint8_t>(param6PerObject.mSize);
                std::memcpy(param6PerObject.mDefaultParams.data(), &matrixTestArr, param6PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param6PerObject);

                //Light
                CB_Info cbInfoLightBuffer;
                cbInfoLightBuffer.mHashName = HashUtils::CharToHashFnv1a("LightBuffer");
                cbInfoLightBuffer.mSlot = 3;
                cbInfoLightBuffer.mSize = sizeof(LightData);

                CB_Param param1LightBuffer;
                param1LightBuffer.mHashName = HashUtils::CharToHashFnv1a("LightData");
                param1LightBuffer.mSize = sizeof(LightData);
                param1LightBuffer.mDefaultParams = std::vector<uint8_t>(param1LightBuffer.mSize);
                LightData ld;
                std::memcpy(param1LightBuffer.mDefaultParams.data(), &ld, param1LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param1LightBuffer);

                mHashToCbInfo[cbInfoPerApplication.mHashName] = cbInfoPerApplication;
                mHashToCbInfo[cbInfoPerFrame.mHashName] = cbInfoPerFrame;
                mHashToCbInfo[cbInfoPerObject.mHashName] = cbInfoPerObject;              
                mHashToCbInfo[cbInfoLightBuffer.mHashName] = cbInfoLightBuffer;

                mConstantBuffers = std::vector<ID3D11Buffer*>(mHashToCbInfo.size());

                // Create the constant buffers for the variables defined in the vertex shader.
                D3D11_BUFFER_DESC constantBufferDesc;
                ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
                constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                constantBufferDesc.CPUAccessFlags = 0;
                constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                HRESULT hr;
                for (const std::pair<const uint32_t, CB_Info>& cb : mHashToCbInfo)
                {
                    uint32_t bufferSize = cb.second.mSize;
                    bufferSize = (bufferSize + 15) & ~15;
                    constantBufferDesc.ByteWidth = bufferSize;
                    hr = NULL;// GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffers[cb.second.mSlot]);
                    if (FAILED(hr))
                    {
                        std::string outMsg("Failed to create Buffer slot: ");
                        outMsg.append(std::to_string(cb.second.mSlot));
                        outMsg.append("\n");
                        Debug::PopError(outMsg.c_str());
                    }

                    for (size_t paramIndex = 0; paramIndex < cb.second.mParams.size(); ++paramIndex)
                    {
                        CB_Param param = cb.second.mParams[paramIndex];
                        RegisterData(param);
                    }
                }
            }

            void RConstantBuffersHandler_DX11::RegisterData(CB_Param param)
            {
                mHashToParam[param.mHashName] = std::malloc(param.mSize);
                std::memcpy(mHashToParam[param.mHashName], param.mDefaultParams.data(), param.mSize);
                mHashParamToSize[param.mHashName] = param.mSize;
            }

            void RConstantBuffersHandler_DX11::SetParamData(uint32_t paramHashName, void* data)
            {
                std::memcpy(mHashToParam[paramHashName], data, mHashParamToSize[paramHashName]);
            }

            void RConstantBuffersHandler_DX11::UpdateSubresource(uint32_t bufferHashName)
            {
                CB_Info& cbInfo = mHashToCbInfo[bufferHashName];
                std::string name = HashUtils::g_hashToStringMap[bufferHashName];

                void* data = std::malloc(cbInfo.mSize);

                uint32_t offset = 0;
                for (CB_Param& param : cbInfo.mParams)
                {
                    std::memcpy(static_cast<char*>(data) + offset, static_cast<char*>(mHashToParam[param.mHashName]), param.mSize);
                    offset += param.mSize;
                }

                //GetDeviceContext()->UpdateSubresource(mConstantBuffers[cbInfo.mSlot], 0, nullptr, data, 0, 0);

                //todo: i think we have a memory link here
                //delete data;
            }

            void RConstantBuffersHandler_DX11::PSSetConstantBuffers()
            {
                //GetDeviceContext()->PSSetConstantBuffers(0, mConstantBuffers.size(), mConstantBuffers.data());
            }

            void RConstantBuffersHandler_DX11::VSSetConstantBuffers()
            {
                //GetDeviceContext()->VSSetConstantBuffers(0, mConstantBuffers.size(), mConstantBuffers.data());
            }
        }
    }
}

