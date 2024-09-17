#include "Components/RDirectionalLightComponent.h"
#include "Graphics/LightManager.h"
#include "Data/RLightData.h"

namespace Faia
{
    namespace Root
    {
        RDirectionalLightComponent::~RDirectionalLightComponent()
        {
            Graphics::Light::GetLightManager()->RemoveEnvironmentLight(mDirectionalLightId);
        }

        void RDirectionalLightComponent::Init()
        {
            Super::Init();
            Graphics::Light::GetLightManager()->RegisterEnvironmentLight(this);
            mOwner->RegisterOnNotifyRotationChange(std::bind(&RDirectionalLightComponent::OnOwnerObjectChangeRotation, this, std::placeholders::_1));
            OnOwnerObjectChangeRotation(mOwner->GetRotation());
        }

        void RDirectionalLightComponent::SetColor(RColorRGB color)
        {
            mData->base.color = color;
        }

        void RDirectionalLightComponent::SetStrength(float s)
        {
            mData->base.strength = s;
        }

        void RDirectionalLightComponent::OnOwnerObjectChangeRotation(RVector3D newRotation)
        {
            float yawRad = Math::AngleToRadians(newRotation.X); 
            float pitchRad = Math::AngleToRadians(newRotation.Y); 

            mData->direction = RVector3D(
                cosf(yawRad) * cosf(pitchRad),
                -sinf(yawRad) * cosf(pitchRad),
                sinf(pitchRad));                

            Debug::Log(mData->direction.ToString().c_str());
        }
    }
}