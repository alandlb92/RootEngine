#pragma once
#include "Data/Vector3D.h"

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

    Quaternion operator* (const float value)
    {
        return Quaternion(X * value, Y * value, Z * value, W * value);
    }

    Quaternion operator+ (const Quaternion value)
    {
        return Quaternion(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
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