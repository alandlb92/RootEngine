#include "Components/PointLightComponent.h"
#include "Graphics/GraphicsMain.h"
#include "Graphics/LightManager.h"

namespace Faia
{
    namespace Root
    {
        void PointLightComponent::SetColor(float r, float g, float b)
        {
            Graphics::Light::LightManager::GetInstance()->SetPointLightColor(RColorRGB(r, g, b));
        }

        void PointLightComponent::SetStrength(float s)
        {
            Graphics::Light::LightManager::GetInstance()->SetPointLightStrength(s);
        }

        void PointLightComponent::OnOwnerObjectChangePosition(Vector3D newPosition)
        {
            Graphics::Light::LightManager::GetInstance()->SetPointLightPosition(newPosition);
        }

        void PointLightComponent::Init()
        {
            Super::Init();
            mOwner->RegisterOnNotifyPositionChange(std::bind(&PointLightComponent::OnOwnerObjectChangePosition, this, std::placeholders::_1));
        }
    }
}