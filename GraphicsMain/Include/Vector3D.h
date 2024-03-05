#pragma once

struct Vector3D
{
    float X, Y, Z;

    Vector3D(float f)
    {
        X = f;
        Y = f;
        Z = f;
    }

    Vector3D(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
};