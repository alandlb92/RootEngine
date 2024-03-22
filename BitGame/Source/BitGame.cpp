// BitGame.cpp : Defines the entry point for the application.
//
#include "BitGame.h"
#include "WindowsApplication.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    WindowsApplication app(hInstance, IDS_APP_TITLE, IDC_BITGAME, IDI_SMALL);
    return app.Run(nCmdShow);
}
