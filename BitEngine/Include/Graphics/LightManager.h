#pragma once

#include "Data/RColor.h"
#include "Data/Vector3D.h"

namespace Graphics 
{
	namespace Light
	{
		struct CB_LightData
		{
			RColorRGB ambientLightColor;
			float ambientLightStrength;

			RColorRGB pointLightColor;
			float offset;

			Vector3D pointLightPosition;
			float pointLightStrength;

			CB_LightData()
			{
				ambientLightColor = RColorRGB(1.0f, 1.0f, 1.0f);
				ambientLightStrength = 1.0f;
				offset = 0.0f;
				pointLightColor = RColorRGB(1.0f, 1.0f, 1.0f);
				pointLightPosition = Vector3D(0.0f, 0.0f, 0.0f);
				pointLightStrength = 0.0f;
			}
		};

		class LightManager
		{
		private:
			CB_LightData* _lightData;
			void UploadLightBuffer();
			
			static LightManager* _instance;
		public:	
			static LightManager* GetInstance() { return _instance; }

			LightManager();
			~LightManager();



			void SetAmbientLightColor(RColorRGB color);
			void SetAmbientLightStrength(float strength);

			void SetPointLightColor(RColorRGB color);
			void SetPointLightPosition(Vector3D position);
			void SetPointLightStrength(float strength);

			const RColorRGB GetAmbientLightColor() { return _lightData->ambientLightColor; };
			const float GetAmbientLightStrength() { return _lightData->ambientLightStrength; };

			const RColorRGB GetPointLightColor() { return _lightData->pointLightColor; };
			const Vector3D GetPointLightPosition() { return _lightData->pointLightPosition; };
			const float GetPointLightStrength() { return _lightData->pointLightStrength; };
		};
	}
}