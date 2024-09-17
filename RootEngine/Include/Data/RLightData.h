#pragma once;

#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 4

#include "RVector3D.h"
#include <sstream>
#include <cmath>

#include "Faia/FMath.h"
#include "Faia/Debug.h"


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
            int active;
        };

        struct LightData
        {
            RColorRGB ambientDownColor;
            int dummyOffset;
            RColorRGB ambientUpColor;
            int dummyOffset1;
            DirectionalLight directionalLight[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

            LightData()
            {
                ambientDownColor = RColorRGB(0.0f, 0.0f, 0.0f);
                ambientUpColor = RColorRGB(1.0f, 1.0f, 1.0f);

                for (int i = 0; i < MAX_NUM_OF_DIRECTIONAL_LIGHTS; ++i)
                {
                    directionalLight[i].base.color = RColorRGB(1.0f, 0.0f, 0.0f);
                    directionalLight[i].base.strength = 1.f;
                    directionalLight[i].direction = RVector3D(0.0f);
                    directionalLight[i].active = 0;
                }
            }

            //todo: ifdef debug
            void DebugDump()
            {
                std::stringstream ss;
                ss << "Light Data: " << "\n"
                    << "Ambient Color: " << ambientDownColor.R << ", " << ambientDownColor.G << ", " << ambientDownColor.B << "\n"
                    << "Ambient Color: " << ambientUpColor.R << ", " << ambientUpColor.G << ", " << ambientUpColor.B << "\n";

                for (int i = 0; i < MAX_NUM_OF_DIRECTIONAL_LIGHTS; ++i)
                {
                    ss << "Directional light " << i << (directionalLight[i].active == 0 ? " DISABLED" : " ENABLED") << ": \n"
                        << "Color: " << directionalLight[i].base.color.R << ", " << directionalLight[i].base.color.G << ", " << directionalLight[i].base.color.B << "\n"
                        << "Strength: " << directionalLight[i].base.strength << "\n"
                        << "Direction: " << directionalLight[i].direction.X << ", " << directionalLight[i].direction.Y << ", " << directionalLight[i].direction.Z << "\n";
                }

                Debug::Log(ss.str().c_str());
            }
        };
    }
}