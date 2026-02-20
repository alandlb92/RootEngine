#pragma once

#include "Data/RColor.h"
#include "Data/RVector3D.h"
#include "Data/RLightData.h"
#include "Components/RDirectionalLightComponent.h"

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			namespace Light
			{	
				class LightManager_DX11
				{
				public:
					LightManager_DX11();
					~LightManager_DX11();

					void SetDirectionalLight(DirectionalLight directionalLight, uint8_t index);					
					DirectionalLight GetDirectionalLight(uint8_t index);

					void UpdateLightToCB();
					void RegisterEnvironmentLight(RDirectionalLightComponent* directionalLightComponent);
					void RemoveEnvironmentLight(uint8_t id);

				private:
					LightData* mLightData;
					uint8_t mDirectionalLightsInScene = 0;					 
					RDirectionalLightComponent* mDirectionalLights[MAX_NUM_OF_DIRECTIONAL_LIGHTS];
				};

				LightManager_DX11* GetLightManager();
			}
		}
	}
}