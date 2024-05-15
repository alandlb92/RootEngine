#include "Graphics/LightManager.h"
#include "Graphics/GraphicsMain.h"

Graphics::Light::LightManager* Graphics::Light::LightManager::_instance = nullptr;

Graphics::Light::LightManager::LightManager()
{
    _lightData = new CB_LightData();
    _instance = this;
}

Graphics::Light::LightManager::~LightManager()
{
    delete _lightData;
    _lightData = nullptr;
}

void Graphics::Light::LightManager::UploadLightBuffer()
{
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Light, _lightData);
}

void Graphics::Light::LightManager::SetAmbientLightColor(RColorRGB color)
{
    _lightData->ambientLightColor = color;
    UploadLightBuffer();
}

void Graphics::Light::LightManager::SetAmbientLightStrength(float strength)
{
    _lightData->ambientLightStrength = strength;
    UploadLightBuffer();
}

void Graphics::Light::LightManager::SetPointLightColor(RColorRGB color)
{
    _lightData->pointLightColor = color;
    UploadLightBuffer();
}

void Graphics::Light::LightManager::SetPointLightPosition(Vector3D position)
{
    _lightData->pointLightPosition = position;
    UploadLightBuffer();
}

void Graphics::Light::LightManager::SetPointLightStrength(float strength)
{
    _lightData->pointLightStrength = strength;
    UploadLightBuffer();
}
