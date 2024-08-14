#pragma once
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
};