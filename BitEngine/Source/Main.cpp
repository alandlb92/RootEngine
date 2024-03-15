// BitEngine.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include "Resource.h"
#include "WindowsApplication.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WindowsApplication app(hInstance);
    return app.Run(nCmdShow);
}