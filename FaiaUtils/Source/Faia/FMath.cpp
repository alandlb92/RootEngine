#include "FMath.h"

namespace Faia
{
    namespace Math 
    {
        const float PI = 3.1415927f;

        float AngleToRadians(float value)
        {
            return value * (PI / 180.0f);
        }
    }
}