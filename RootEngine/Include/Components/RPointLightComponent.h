#pragma once
#include "Graphics/GraphicsMain.h"
#include "Core/RComponent.h"


namespace Faia
{
	namespace Root
	{
		class RPointLightComponent : public RComponent
		{
			typedef RComponent Super;

		public:
			void SetColor(float r, float g, float b);
			void SetStrength(float s);

		protected:
			void OnOwnerObjectChangePosition(RVector3D newPosition);
			void Init() override;
		};
	}
}