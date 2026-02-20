#include "Graphics/DX11/LightManager_DX11.h"
#include "Graphics/DX11/RConstantBuffersHandler_DX11.h"
#include "Faia/Debug.h"

namespace Faia
{
    namespace Root
    {
        namespace Graphics
        {
            namespace Light
            {
                LightManager_DX11* gLightManager = nullptr;

                LightManager_DX11* GetLightManager()
                {
                    if (!gLightManager)
                    {
                        gLightManager = new LightManager_DX11();
                    }

                    return gLightManager;
                }

                LightManager_DX11::LightManager_DX11()
                {
                    mLightData = new LightData();
                }

                LightManager_DX11::~LightManager_DX11()
                {
                    delete mLightData;
                    mLightData = nullptr;
                }

                void LightManager_DX11::SetDirectionalLight(DirectionalLight directionalLight, uint8_t index)
                {
                    mLightData->directionalLight[index] = directionalLight;
                }

                DirectionalLight LightManager_DX11::GetDirectionalLight(uint8_t index)
                {
                    //todo: Implement assert
                    if (index >= MAX_NUM_OF_DIRECTIONAL_LIGHTS)
                    {
                        Debug::PopError("You trying to put more directional light than the max");
                    }

                    return mLightData->directionalLight[index];
                }

                void LightManager_DX11::UpdateLightToCB()
                {
                    Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gLightData, mLightData);
                    Graphics::GetConstantBuffersHandler()->UpdateSubresource(Graphics::gLightBufferHash);
                    //mLightData->DebugDump();
                }

                void LightManager_DX11::RegisterEnvironmentLight(RDirectionalLightComponent* directionalLightComponent)
                {
                    int id = mDirectionalLightsInScene;

                    if (mDirectionalLightsInScene >= MAX_NUM_OF_DIRECTIONAL_LIGHTS)
                    {
                        Debug::PopError("You exced the max number of directional lights per scene");
                        return;
                    }


                    directionalLightComponent->mData = &mLightData->directionalLight[id];
                    directionalLightComponent->mDirectionalLightId = id;
                    mLightData->directionalLight[id].active = 1;
                    mDirectionalLights[id] = directionalLightComponent;
                    mDirectionalLightsInScene++;
                }

                void LightManager_DX11::RemoveEnvironmentLight(uint8_t id)
                {
                    mDirectionalLightsInScene--;

                    if (id != MAX_NUM_OF_DIRECTIONAL_LIGHTS - 1)
                    {
                       mLightData->directionalLight[id].active = 0;
                       std::swap(mLightData->directionalLight[id], mLightData->directionalLight[MAX_NUM_OF_DIRECTIONAL_LIGHTS - 1]);
                       std::swap(mDirectionalLights[id], mDirectionalLights[MAX_NUM_OF_DIRECTIONAL_LIGHTS - 1]);
                       mDirectionalLights[MAX_NUM_OF_DIRECTIONAL_LIGHTS - 1] = nullptr;
                    }
                }
            }
        }
    }
}