#pragma once
#include "REConfiguration.h"

class REWindow
{
public:
    REWindow(REConfiguration config);
    virtual void Update() = 0;
    void CalculateWindowSize();

    float GetWidth() { return mWidth; }
    float GetHeight() { return mHeight; }

protected:
    float mWidthPercentage;
    float mHeightPercentage;
    float mPositionX;
    float mPositionY;

    float mWidth;
    float mHeight;
};

