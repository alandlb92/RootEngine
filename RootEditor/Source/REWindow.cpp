#include "REWindow.h"
#include "Faia/WindowsApplication.h"

REWindow::REWindow(REConfiguration config)
{
    mWidthPercentage = config.mWidthPercentage;
    mHeightPercentage = config.mHeightPercentage;
    mPositionX = config.mPositionX;
    mPositionY = config.mPositionY;
    CalculateWindowSize();
}

void REWindow::CalculateWindowSize()
{
    mWidth = Faia::Windows::GetWinApp()->GetWidth() * mWidthPercentage;
    mHeight = Faia::Windows::GetWinApp()->GetHeight() * mHeightPercentage;
}
