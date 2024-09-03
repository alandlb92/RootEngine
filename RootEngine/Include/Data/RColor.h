#pragma once
#include <iostream>

struct RColorRGB
{
    float R;
    float G;
    float B;

    RColorRGB()
    {
        R = 1;
        G = 1;
        B = 1;
    };

    RColorRGB(float r, float g, float b)
    {
        R = r;
        G = g;
        B = b;
    };
};


struct RColorRGBA : public RColorRGB
{
    float A;

    RColorRGBA() : RColorRGB()
    {
        A = 1;
    };

    RColorRGBA(float r, float g, float b, float a) : RColorRGB(r, g, b)
    {
        A = a;
    };
};


struct RColorRGBA_8b
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;

    RColorRGBA_8b()
    {
        R = 255;
        G = 255;
        B = 255;
        A = 255;
    };

    RColorRGBA_8b(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    };

    bool operator==(const RColorRGBA_8b& value) const
    {
        return R == value.R && G == value.G && B == value.B && A == value.A;
    }
};