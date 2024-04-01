#pragma once
#include "Data/Vector3D.h"

struct Vector4D : public Vector3D
{
    float W;

    Vector4D() : Vector3D()
    {
        W = 1;
    }

    Vector4D(float f) : Vector3D(f)
    {
        W = f;
    }

    Vector4D(float x, float y, float z, float w) : Vector3D(x, y, z)
    {
        W = w;
    }

    Vector4D operator* (const float value)
    {
        return Vector4D(X * value, Y * value, Z * value, W * value);
    }

    Vector4D operator+ (const Vector4D value)
    {
        return Vector4D(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
    }

    Vector4D& operator += (const Vector4D value)
    {
        this->X += value.X;
        this->Y += value.Y;
        this->Z += value.Z;
        this->W += value.W;
        return *this;
    }
};