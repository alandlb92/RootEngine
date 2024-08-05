#pragma once
#include "Data/Vector3D.h"

namespace Faia
{
    namespace Root
    {
        struct Quaternion : public Vector3D
        {
            float W;

            Quaternion() : Vector3D()
            {
                W = 1;
            }

            Quaternion(float f) : Vector3D(f)
            {
                W = f;
            }

            Quaternion(float x, float y, float z, float w) : Vector3D(x, y, z)
            {
                W = w;
            }

            bool operator==(const Quaternion& value) const
            {
                return X == value.X && Y == value.Y && Z == value.Z && W == value.W;
            }

            Quaternion operator* (const float value)
            {
                return Quaternion(X * value, Y * value, Z * value, W * value);
            }

            Quaternion operator+ (const Quaternion value)
            {
                return Quaternion(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
            }

            //TODO: maibe change this to a Math lib
            static Quaternion Lerp(Quaternion start, Quaternion end, float t)
            {
                Quaternion result(
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

            Quaternion& operator += (const Quaternion value)
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