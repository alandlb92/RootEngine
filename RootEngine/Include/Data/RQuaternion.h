#pragma once
#include "Data/RVector3D.h"

namespace Faia
{
    namespace Root
    {
        struct RQuaternion : public RVector3D
        {
            float W;

            RQuaternion() : RVector3D()
            {
                W = 1.f;
            }

            RQuaternion(float f) : RVector3D(f)
            {
                W = f;
            }

            RQuaternion(float x, float y, float z, float w) : RVector3D(x, y, z)
            {
                W = w;
            }

            bool operator==(const RQuaternion& value) const
            {
                return X == value.X && Y == value.Y && Z == value.Z && W == value.W;
            }

            RQuaternion operator* (const float value)
            {
                return RQuaternion(X * value, Y * value, Z * value, W * value);
            }

            RQuaternion operator+ (const RQuaternion value)
            {
                return RQuaternion(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
            }

            //TODO: maibe change this to a Math lib and do a test
            static RQuaternion Lerp(RQuaternion start, RQuaternion end, float t)
            {
                RQuaternion result(
                    start.X + t * (end.X - start.X),
                    start.Y + t * (end.Y - start.Y),
                    start.Z + t * (end.Z - start.Z),
                    start.W + t * (end.W - start.W)
                );

                // Normaliza o quaternion resultante para garantir que ele seja unitário
                float magnitude = sqrt(result.X * result.X + result.Y * result.Y + result.Z * result.Z + result.W * result.W);
                if (magnitude > 0.0f) {
                    result.X /= magnitude;
                    result.Y /= magnitude;
                    result.Z /= magnitude;
                    result.W /= magnitude;
                }

                return result;
            }

            RQuaternion& operator += (const RQuaternion value)
            {
                this->X += value.X;
                this->Y += value.Y;
                this->Z += value.Z;
                this->W += value.W;
                return *this;
            }
        };
    }
}