#pragma once
#include "framework.h"
#include "Resource.h"
#include <memory>
#include "InputSystem.h"

#define MAX_LOADSTRING 100

class WindowsApplication {
public:
    WindowsApplication(HINSTANCE hInstance);
    ~WindowsApplication();

    int Run(int nCmdShow);

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static WindowsApplication* _appInstance;

    HINSTANCE hInstance;
    HWND windowHandler;
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
    std::unique_ptr<InputSystem> _inputSystem;

    ATOM MyRegisterClass();
    BOOL InitInstance(int nCmdShow);
};