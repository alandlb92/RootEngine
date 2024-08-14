#include "Components/RPointLightComponent.h"
#include "Graphics/GraphicsMain.h"
#include "Graphics/LightManager.h"

namespace Faia
{
    namespace Root
    {
        void RPointLightComponent::SetColor(float r, float g, float b)
        {
            Graphics::Light::LightManager::GetInstance()->SetPointLightColor(RColorRGB(r, g, b));
        }

        void RPointLightComponent::SetStrength(float s)
        {
            Graphics::Light::LightManager::GetInstance()->SetPointLightStrength(s);
        }

        void RPointLightComponent::OnOwnerObjectChangePosition(RVector3D newPosition)
        {
            Graphics::Light::LightManager::GetInstance()->SetPointLightPosition(newPosition);
        }

        void RPointLightComponent::Init()
        {
            Super::Init();
            mOwner->RegisterOnNotifyPositionChange(std::bind(&RPointLightComponent::OnOwnerObjectChangePosition, this, std::placeholders::_1));
        }
    }
}