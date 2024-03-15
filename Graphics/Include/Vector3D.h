#pragma once
#include "Vector2D.h"

struct Vector3D  : public Vector2D
{
    float Z;

    Vector3D(float f) : Vector2D(f)
    {
        Z = f;
    }

    Vector3D(float x, float y, float z) : Vector2D(x, y)
    {
        Z = z;
    }

    Vector3D operator* (const float value)
    {
        return Vector3D(X * value, Y * value, Z * value);
    }
};