#pragma once
#include <memory>
#include "FaiaInputSystem.h"


#define MAX_LOADSTRING 100

class WindowsApplication {

public:
    WindowsApplication(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall);
    ~WindowsApplication();

    int Run(int nCmdShow);

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static WindowsApplication* _appInstance;

    UINT _idcSamall;
    UINT _idsAppTittle;
    UINT _idcGame;

    HINSTANCE hInstance;
    HWND windowHandler;
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
    std::unique_ptr<Faia::InputSystem::FaiaInputSystem> _inputSystem;

    ATOM MyRegisterClass();
    BOOL InitInstance(int nCmdShow);
};