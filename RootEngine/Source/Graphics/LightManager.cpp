#include "Graphics/LightManager.h"
#include "Graphics/RConstantBuffersHandler.h"

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
                    _lightData = new CB_LightData();
                    _instance = this;
                }

                LightManager::~LightManager()
                {
                    delete _lightData;
                    _lightData = nullptr;
                }

                void LightManager::SetAmbientLightColor(RColorRGB color)
                {
                    _lightData->ambientLightColor = color;
                    GetConstantBuffersHandler()->SetParamData(gAmbientLightColorHash, &_lightData->ambientLightColor);
                }

                void LightManager::SetAmbientLightStrength(float strength)
                {
                    _lightData->ambientLightStrength = strength;
                    GetConstantBuffersHandler()->SetParamData(gAmbientLightStrengthHash, &_lightData->ambientLightStrength);
                }

                void LightManager::SetPointLightColor(RColorRGB color)
                {
                    _lightData->pointLightColor = color;
                    GetConstantBuffersHandler()->SetParamData(gPointLightColorHash, &_lightData->pointLightColor);
                }

                void LightManager::SetPointLightPosition(RVector3D position)
                {
                    _lightData->pointLightPosition = position;
                    GetConstantBuffersHandler()->SetParamData(gPointLightpositionHash, &_lightData->pointLightPosition);
                }

                void LightManager::SetPointLightStrength(float strength)
                {
                    _lightData->pointLightStrength = strength;
                    GetConstantBuffersHandler()->SetParamData(gPointLightStrengthHash, &_lightData->pointLightStrength);
                }
            }
        }
    }
}