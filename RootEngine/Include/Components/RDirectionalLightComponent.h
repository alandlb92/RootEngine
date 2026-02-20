#pragma once
#include "Core/RComponent.h"
#include "Data/RLightData.h"
#include "Data/RColor.h"

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			namespace Light
			{
				class LightManager_DX11;
			}
		}

		class RDirectionalLightComponent : public RComponent
		{
			typedef RComponent Super;
			friend class Graphics::Light::LightManager_DX11;

		public:
			~RDirectionalLightComponent();
			void Init() override;

			void SetColor(RColorRGB color);
			void SetStrength(float s);

		protected:
			void OnOwnerObjectChangeRotation(RVector3D newRotation);
			uint8_t mDirectionalLightId;
			DirectionalLight* mData;
		};
	}
}