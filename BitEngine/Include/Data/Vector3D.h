#pragma once
#include "Vector2D.h"

struct Vector3D  : public Vector2D
{
    float Z;

    Vector3D() : Vector2D()
    {
        Z = 0;
    }

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

    Vector3D operator* (const Vector3D value)
    {
        return Vector3D(X * value.X, Y * value.Y, Z * value.Z);
    }

    Vector3D operator+ (const Vector3D value)
    {
        return Vector3D(X + value.X, Y + value.Y, Z + value.Z);
    }

    Vector3D& operator += (const Vector3D value)
    {
        this->X += value.X;
        this->Y += value.Y;
        this->Z += value.Z;
        return *this;
    }

    //TODO: maibe change this to a Math lib
    static Vector3D Lerp(Vector3D start, Vector3D end, float t)
    {
        return Vector3D(
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