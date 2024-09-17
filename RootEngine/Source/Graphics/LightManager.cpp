#include "Graphics/LightManager.h"
#include "Graphics/RConstantBuffersHandler.h"
#include "Faia/Debug.h"

namespace Faia
{
    namespace Root
    {
        namespace Graphics
        {
            namespace Light
            {
                LightManager* gLightManager = nullptr;

                LightManager* GetLightManager()
                {
                    if (!gLightManager)
                    {
                        gLightManager = new LightManager();
                    }

                    return gLightManager;
                }

                LightManager::LightManager()
                {
                    mLightData = new LightData();
                }

                LightManager::~LightManager()
                {
                    delete mLightData;
                    mLightData = nullptr;
                }

                void LightManager::SetDirectionalLight(DirectionalLight directionalLight, uint8_t index)
                {
                    mLightData->directionalLight[index] = directionalLight;
                }

                DirectionalLight LightManager::GetDirectionalLight(uint8_t index)
                {
                    //todo: Implement assert
                    if (index >= MAX_NUM_OF_DIRECTIONAL_LIGHTS)
                    {
                        Debug::PopError("You trying to put more directional light than the max");
                    }

                    return mLightData->directionalLight[index];
                }

                void LightManager::UpdateLightToCB()
                {
                    Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gLightData, mLightData);
                    Graphics::GetConstantBuffersHandler()->UpdateSubresource(Graphics::gLightBufferHash);
                    //mLightData->DebugDump();
                }

                void LightManager::RegisterEnvironmentLight(RDirectionalLightComponent* directionalLightComponent)
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

                void LightManager::RemoveEnvironmentLight(uint8_t id)
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