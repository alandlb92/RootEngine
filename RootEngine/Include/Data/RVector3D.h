#pragma once
#include "RVector2D.h"

namespace Faia
{
    namespace Root
    {
        struct RVector3D : public RVector2D
        {
            float Z;

            RVector3D() : RVector2D()
            {
                Z = 0;
            }

            RVector3D(float f) : RVector2D(f)
            {
                Z = f;
            }

            RVector3D(float x, float y, float z) : RVector2D(x, y)
            {
                Z = z;
            }

            RVector3D operator* (const float value)
            {
                return RVector3D(X * value, Y * value, Z * value);
            }

            RVector3D operator* (const RVector3D value)
            {
                return RVector3D(X * value.X, Y * value.Y, Z * value.Z);
            }

            RVector3D operator+ (const RVector3D value)
            {
                return RVector3D(X + value.X, Y + value.Y, Z + value.Z);
            }

            RVector3D& operator += (const RVector3D value)
            {
                this->X += value.X;
                this->Y += value.Y;
                this->Z += value.Z;
                return *this;
            }

            bool operator==(const RVector3D& value) const
            {
                return X == value.X && Y == value.Y && Z == value.Z;
            }

            //TODO: maibe change this to a Math lib and add to Math lib to Vec operations
            static RVector3D Lerp(RVector3D start, RVector3D end, float t)
            {
                return RVector3D(
                    start.X + t * (end.X - start.X),
                    start.Y + t * (end.Y - start.Y),
                    start.Z + t * (end.Z - start.Z)
                );
            }

            std::string ToString()
            {
                std::string s("X: ");
                s.append(std::to_string(X));
                s.append("\n");
                s.append("Y: ");
                s.append(std::to_string(Y));
                s.append("\n");
                s.append("Z: ");
                s.append(std::to_string(Z));
                s.append("\n");
                return s;
            }
        };
    }
}