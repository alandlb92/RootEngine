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
                LightManager* LightManager::_instance = nullptr;

                LightManager::LightManager()
                {
                    lightData = new LightData();
                    _instance = this;
                }

                LightManager::~LightManager()
                {
                    delete lightData;
                    lightData = nullptr;
                }

                void LightManager::SetAmbientLightColor(RColorRGB color)
                {
                    lightData->ambientLightColor = color;
                }

                void LightManager::SetAmbientLightStrength(float strength)
                {
                    lightData->ambientLightStrength = strength;
                }

                void LightManager::SetDirectionalLight(DirectionalLight directionalLight, uint8_t index)
                {
                    lightData->directionalLight[index] = directionalLight;
                }

                DirectionalLight LightManager::GetDirectionalLight(uint8_t index)
                {
                    //todo: Implement assert
                    if (index >= MAX_NUM_OF_DIRECTIONAL_LIGHTS)
                    {
                        Debug::PopError("You trying to put more directional light than the max");
                    }

                    return lightData->directionalLight[index];
                }

                void LightManager::UpdateLightToCB()
                {
                    Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gLightData, lightData);
                    Graphics::GetConstantBuffersHandler()->UpdateSubresource(Graphics::gLightBufferHash);
                }
            }
        }
    }
}