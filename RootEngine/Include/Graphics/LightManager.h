#pragma once

#include "Data/RColor.h"
#include "Data/RVector3D.h"
#include "Data/RLightData.h"

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			namespace Light
			{				

				class LightManager
				{
				private:
					LightData* lightData;
					static LightManager* _instance;

				public:
					static LightManager* GetInstance() { return _instance; }

					LightManager();
					~LightManager();

					void SetAmbientLightColor(RColorRGB color);
					void SetAmbientLightStrength(float strength);
					void SetDirectionalLight(DirectionalLight directionalLight, uint8_t index);
					
					const RColorRGB GetAmbientLightColor() { return lightData->ambientLightColor; };
					const float GetAmbientLightStrength() { return lightData->ambientLightStrength; };
					DirectionalLight GetDirectionalLight(uint8_t index);

					void UpdateLightToCB();
				};
			}
		}
	}
}