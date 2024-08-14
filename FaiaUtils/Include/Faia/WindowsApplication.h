#pragma once
#include <windows.h>
#include <memory>
#include <functional>
#include <vector>

#define MAX_LOADSTRING 100

namespace Faia
{
    namespace Windows
    {
        using NotifyWinProc = std::function<void(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>;
        using NotifyUpdate = std::function<void()>;

        //Todo: maibe move that class to utils
        class WindowsApplication {

        public:
            WindowsApplication(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall);
            ~WindowsApplication();

            void RegisterWinAppProcFunction(NotifyWinProc winprocFunction);
            void RegisterWinAppUpdateFunction(NotifyUpdate appUpdateFunction);

            int Run(int nCmdShow);
            HWND GetWindowHandler();
            BOOL InitInstance(int nCmdShow);

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

            ATOM RegisterClass();

            std::vector<NotifyWinProc> mFunctionsNotifyAppWinProc;
            std::vector<NotifyUpdate> mFunctionsNotifyAppUpdate;
        };

    }
}