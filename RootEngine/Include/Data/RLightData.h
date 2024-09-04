#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 4

#include "RVector3D.h"

namespace Faia
{
    namespace Root
    {
        struct BaseLight
        {
            RColorRGB color;
            float strength;
        };

        struct DirectionalLight
        {
            BaseLight base;
            RVector3D direction;
            int offset;
        };

        struct LightData
        {
            RColorRGB ambientLightColor;
            float ambientLightStrength;
            DirectionalLight directionalLight[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

            LightData()
            {
                ambientLightColor = RColorRGB(1.0f, 1.0f, 1.0f);
                ambientLightStrength = 1.0f;

                for (int i = 0; i < MAX_NUM_OF_DIRECTIONAL_LIGHTS; ++i)
                {
                    directionalLight[i].base.color = RColorRGB(1.0f, 1.0f, 1.0f);
                    directionalLight[i].base.strength = 1.0f;
                    directionalLight[i].direction = RVector3D(.0f, 90.f, .0f);
                }
            }
        };
    }
}