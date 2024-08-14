#pragma once
#include <string>

namespace Faia
{
    namespace Root
    {
        struct RVector2D
        {
            float X, Y;

            RVector2D()
            {
                X = 0;
                Y = 0;
            }

            RVector2D(float f)
            {
                X = f;
                Y = f;
            }

            RVector2D(float x, float y)
            {
                X = x;
                Y = y;
            }

            RVector2D operator* (const float value)
            {
                return RVector2D(X * value, Y * value);
            }

            RVector2D operator+ (const float value)
            {
                return RVector2D(X + value, Y + value);
            }


            bool operator==(const RVector2D& value) const 
            {
                return X == value.X && Y == value.Y;
            }

            RVector2D& operator += (const RVector2D value)
            {
                this->X += value.X;
                this->Y += value.Y;
                return *this;
            }

            std::string ToString()
            {
                std::string s("X: ");
                s.append(std::to_string(X));
                s.append("\n");
                s.append("Y: ");
                s.append(std::to_string(Y));
                s.append("\n");
                return s;
            }
        };
    }
}