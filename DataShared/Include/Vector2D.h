#pragma once

struct Vector2D
{
    float X, Y;

    Vector2D()
    {
        X = 0;
        Y = 0;
    }

    Vector2D(float f)
    {
        X = f;
        Y = f;
    }

    Vector2D(float x, float y)
    {
        X = x;
        Y = y;
    }

    Vector2D operator* (const float value)
    {
        return Vector2D(X * value, Y * value);
    }

    Vector2D operator+ (const float value)
    {
        return Vector2D(X + value, Y + value);
    }

    Vector2D& operator += (const Vector2D value)
    {
        this->X += value.X;
        this->Y += value.Y;
        return *this;
    }
};