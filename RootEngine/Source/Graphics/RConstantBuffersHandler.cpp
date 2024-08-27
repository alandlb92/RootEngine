#include "Graphics/RConstantBuffersHandler.h"
#include "Graphics/GraphicsMain.h"
#include "d3d11.h"
#include "Faia/Debug.h"
#include "Data/RColor.h"
#include "Graphics/LightManager.h"

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
            uint32_t gPerObjectHash;
            uint32_t gWorldMatrixHash;
            uint32_t gAnimMatrixHash;
            uint32_t gGlobalsHash;
            uint32_t gHasTextureHash;
            uint32_t gBoneSelectedIdHash;
            uint32_t gLightBufferHash;
            uint32_t gAmbientLightColorHash;
            uint32_t gAmbientLightStrengthHash;
            uint32_t gPointLightColorHash;
            uint32_t gPointLightpositionHash;
            uint32_t gPointLightStrengthHash;

            void InitializeGHashs()
            {
                gPerApplicationHash = HashUtils::CharToHashFnv1a("PerApplication");
                gProjectionMatrixHash = HashUtils::CharToHashFnv1a("ProjectionMatrix");
                gPerFrameHash = HashUtils::CharToHashFnv1a("PerFrame");
                gViewMatrixHash = HashUtils::CharToHashFnv1a("ViewMatrix");
                gPerObjectHash = HashUtils::CharToHashFnv1a("PerObject");
                gWorldMatrixHash = HashUtils::CharToHashFnv1a("WorldMatrix");
                gAnimMatrixHash = HashUtils::CharToHashFnv1a("AnimMatrix");
                gGlobalsHash = HashUtils::CharToHashFnv1a("Globals");
                gHasTextureHash = HashUtils::CharToHashFnv1a("HasTexture");
                gBoneSelectedIdHash = HashUtils::CharToHashFnv1a("BoneSelectedId");
                gLightBufferHash = HashUtils::CharToHashFnv1a("LightBuffer");
                gAmbientLightColorHash = HashUtils::CharToHashFnv1a("AmbientLightColor");
                gAmbientLightStrengthHash = HashUtils::CharToHashFnv1a("AmbientLightStrength");
                gPointLightColorHash = HashUtils::CharToHashFnv1a("PointLightColor");
                gPointLightpositionHash = HashUtils::CharToHashFnv1a("PointLightposition");
                gPointLightStrengthHash = HashUtils::CharToHashFnv1a("PointLightStrength");

            }

            RConstantBuffersHandler* gConstantBuffersHandler;
            RConstantBuffersHandler* GetConstantBuffersHandler()
            {
                if (gConstantBuffersHandler == nullptr)
                {
                    gConstantBuffersHandler = new RConstantBuffersHandler();
                    gConstantBuffersHandler->Initialize();
                }

                return gConstantBuffersHandler;
            }

            void RConstantBuffersHandler::Initialize()
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
                cbInfoPerFrame.mSize = 64;
                CB_Param param1PerFrame;
                param1PerFrame.mHashName = HashUtils::CharToHashFnv1a("ViewMatrix");
                param1PerFrame.mSize = 64;
                param1PerFrame.mDefaultParams = std::vector<uint8_t>(param1PerFrame.mSize);
                std::memcpy(param1PerFrame.mDefaultParams.data(), &matrixTest, param1PerFrame.mSize);
                cbInfoPerFrame.mParams.push_back(param1PerFrame);

                //Per Object
                CB_Info cbInfoPerObject;
                cbInfoPerObject.mHashName = HashUtils::CharToHashFnv1a("PerObject");
                cbInfoPerObject.mSlot = 2;
                cbInfoPerObject.mSize = 64 + (64 * MAX_NUM_OF_ANIMATION_CHANNELS);

                CB_Param param1PerObject;
                param1PerObject.mHashName = HashUtils::CharToHashFnv1a("WorldMatrix");
                param1PerObject.mSize = 64;
                param1PerObject.mDefaultParams = std::vector<uint8_t>(param1PerObject.mSize);
                std::memcpy(param1PerObject.mDefaultParams.data(), &matrixTest, param1PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param1PerObject);


                CB_Param param2PerObject;
                param2PerObject.mHashName = HashUtils::CharToHashFnv1a("AnimMatrix");
                param2PerObject.mSize = 64 * MAX_NUM_OF_ANIMATION_CHANNELS;

                RMatrix4x4 matrixTestArr[MAX_NUM_OF_ANIMATION_CHANNELS];
                for (int i = 0; i < MAX_NUM_OF_ANIMATION_CHANNELS; ++i)
                {
                    matrixTestArr[i] = RMatrix4x4::Identity();
                }
                param2PerObject.mDefaultParams = std::vector<uint8_t>(param2PerObject.mSize);
                std::memcpy(param2PerObject.mDefaultParams.data(), &matrixTestArr, param2PerObject.mSize);
                cbInfoPerObject.mParams.push_back(param2PerObject);

                //Globals
                CB_Info cbInfoGlobals;
                cbInfoGlobals.mHashName = HashUtils::CharToHashFnv1a("Globals");
                cbInfoGlobals.mSlot = 3;
                cbInfoGlobals.mSize = 16;

                CB_Param param1Globals;
                param1Globals.mHashName = HashUtils::CharToHashFnv1a("HasTexture");
                param1Globals.mSize = 4;
                uint32_t intTest = 0;
                param1Globals.mDefaultParams = std::vector<uint8_t>(param1Globals.mSize);
                std::memcpy(param1Globals.mDefaultParams.data(), &intTest, param1Globals.mSize);
                cbInfoGlobals.mParams.push_back(param1Globals);

                CB_Param param2Globals;
                param2Globals.mHashName = HashUtils::CharToHashFnv1a("BoneSelectedId");
                param2Globals.mSize = 4;
                param2Globals.mDefaultParams = std::vector<uint8_t>(param2Globals.mSize);
                std::memcpy(param2Globals.mDefaultParams.data(), &intTest, param2Globals.mSize);
                cbInfoGlobals.mParams.push_back(param2Globals);


                //Light
                CB_Info cbInfoLightBuffer;
                cbInfoLightBuffer.mHashName = HashUtils::CharToHashFnv1a("LightBuffer");
                cbInfoLightBuffer.mSlot = 4;
                cbInfoLightBuffer.mSize = 48;

                CB_Param param1LightBuffer;
                param1LightBuffer.mHashName = HashUtils::CharToHashFnv1a("AmbientLightColor");
                param1LightBuffer.mSize = 12;
                RVector3D vector3Test(1);
                param1LightBuffer.mDefaultParams = std::vector<uint8_t>(param1LightBuffer.mSize);
                std::memcpy(param1LightBuffer.mDefaultParams.data(), &vector3Test, param1LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param1LightBuffer);

                CB_Param param2LightBuffer;
                param2LightBuffer.mHashName = HashUtils::CharToHashFnv1a("AmbientLightStrength");
                param2LightBuffer.mSize = 4;
                float floatTest = 0;
                param2LightBuffer.mDefaultParams = std::vector<uint8_t>(param2LightBuffer.mSize);
                std::memcpy(param2LightBuffer.mDefaultParams.data(), &floatTest, param2LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param2LightBuffer);

                CB_Param param3LightBuffer;
                param3LightBuffer.mHashName = HashUtils::CharToHashFnv1a("PointLightColor");
                param3LightBuffer.mSize = 12;
                param3LightBuffer.mDefaultParams = std::vector<uint8_t>(param3LightBuffer.mSize);
                std::memcpy(param3LightBuffer.mDefaultParams.data(), &vector3Test, param3LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param3LightBuffer);

                CB_Param param4LightBuffer;
                param4LightBuffer.mHashName = HashUtils::CharToHashFnv1a("Offset");
                param4LightBuffer.mSize = 4;
                param4LightBuffer.mDefaultParams = std::vector<uint8_t>(param4LightBuffer.mSize);
                std::memcpy(param4LightBuffer.mDefaultParams.data(), &floatTest, param4LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param4LightBuffer);

                CB_Param param5LightBuffer;
                param5LightBuffer.mHashName = HashUtils::CharToHashFnv1a("PointLightposition");
                param5LightBuffer.mSize = 12;
                param5LightBuffer.mDefaultParams = std::vector<uint8_t>(param5LightBuffer.mSize);
                std::memcpy(param5LightBuffer.mDefaultParams.data(), &vector3Test, param5LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param5LightBuffer);

                CB_Param param6LightBuffer;
                param6LightBuffer.mHashName = HashUtils::CharToHashFnv1a("PointLightStrength");
                param6LightBuffer.mSize = 4;
                param6LightBuffer.mDefaultParams = std::vector<uint8_t>(param6LightBuffer.mSize);
                std::memcpy(param6LightBuffer.mDefaultParams.data(), &floatTest, param6LightBuffer.mSize);
                cbInfoLightBuffer.mParams.push_back(param6LightBuffer);

                mHashToCbInfo[cbInfoPerApplication.mHashName] = cbInfoPerApplication;
                mHashToCbInfo[cbInfoPerFrame.mHashName] = cbInfoPerFrame;
                mHashToCbInfo[cbInfoPerObject.mHashName] = cbInfoPerObject;
                mHashToCbInfo[cbInfoGlobals.mHashName] = cbInfoGlobals;
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
                    constantBufferDesc.ByteWidth = cb.second.mSize;
                    hr = GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffers[cb.second.mSlot]);
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

            void RConstantBuffersHandler::RegisterData(CB_Param param)
            {
                mHashToParam[param.mHashName] = std::malloc(param.mSize);
                std::memcpy(mHashToParam[param.mHashName], &param.mDefaultParams, param.mSize);
                mHashParamToSize[param.mHashName] = param.mSize;
            }

            void RConstantBuffersHandler::SetParamData(uint32_t paramHashName, void* data)
            {
                std::memcpy(mHashToParam[paramHashName], data, mHashParamToSize[paramHashName]);

                if (paramHashName == gProjectionMatrixHash)
                {
                    auto& test = mHashToCbInfo[paramHashName];
                    RMatrix4x4 pMat;
                    std::memcpy(&pMat, mHashToParam[paramHashName], mHashParamToSize[paramHashName]);
                    std::string paramName = HashUtils::g_hashToStringMap[paramHashName];
                }
            }

            void RConstantBuffersHandler::UpdateSubresource(uint32_t bufferHashName)
            {
                CB_Info& cbInfo = mHashToCbInfo[bufferHashName];

                void* data = std::malloc(cbInfo.mSize);

                uint32_t offset = 0;
                for (CB_Param& param : cbInfo.mParams)
                {
                    std::memcpy(static_cast<char*>(data) + offset, static_cast<char*>(mHashToParam[param.mHashName]), param.mSize);
                    offset += param.mSize;
                }

                GetDeviceContext()->UpdateSubresource(mConstantBuffers[cbInfo.mSlot], 0, nullptr, data, 0, 0);

                if (bufferHashName == gLightBufferHash)
                {
                    Light::CB_LightData test;
                    std::memcpy(&test, data, cbInfo.mSize);
                    std::string bufferName = HashUtils::g_hashToStringMap[bufferHashName];
                }

                //delete data;
            }

            void RConstantBuffersHandler::PSSetConstantBuffers()
            {
                GetDeviceContext()->PSSetConstantBuffers(0, mConstantBuffers.size(), mConstantBuffers.data());
            }

            void RConstantBuffersHandler::VSSetConstantBuffers()
            {
                GetDeviceContext()->VSSetConstantBuffers(0, mConstantBuffers.size(), mConstantBuffers.data());
            }
        }
    }
}

