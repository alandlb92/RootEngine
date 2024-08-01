#include "Graphics/LightManager.h"
#include "Graphics/GraphicsMain.h"

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

                void LightManager::UploadLightBuffer()
                {
                    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Light, _lightData);
                }

                void LightManager::SetAmbientLightColor(RColorRGB color)
                {
                    _lightData->ambientLightColor = color;
                    UploadLightBuffer();
                }

                void LightManager::SetAmbientLightStrength(float strength)
                {
                    _lightData->ambientLightStrength = strength;
                    UploadLightBuffer();
                }

                void LightManager::SetPointLightColor(RColorRGB color)
                {
                    _lightData->pointLightColor = color;
                    UploadLightBuffer();
                }

                void LightManager::SetPointLightPosition(Vector3D position)
                {
                    _lightData->pointLightPosition = position;
                    UploadLightBuffer();
                }

                void LightManager::SetPointLightStrength(float strength)
                {
                    _lightData->pointLightStrength = strength;
                    UploadLightBuffer();
                }
            }
        }
    }
}