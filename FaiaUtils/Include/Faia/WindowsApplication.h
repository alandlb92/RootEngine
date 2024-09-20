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
            friend void InitializeApp(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall, int nCmdShow);
            friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        public:
            ~WindowsApplication();

            void RegisterWinAppProcFunction(NotifyWinProc winprocFunction);
            void RegisterWinAppUpdateFunction(NotifyUpdate appUpdateFunction);

            int Run(int nCmdShow);

        private:
            WindowsApplication(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall);
            BOOL InitInstance(int nCmdShow);

            UINT _idiSmall;
            UINT _idsAppTittle;
            UINT _idcGame;

            HINSTANCE hInstance;
            WCHAR szTitle[MAX_LOADSTRING];
            WCHAR szWindowClass[MAX_LOADSTRING];

            ATOM RegisterClass();

            std::vector<NotifyWinProc> mFunctionsNotifyAppWinProc;
            std::vector<NotifyUpdate> mFunctionsNotifyAppUpdate;
        };


        void InitializeApp(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall, int nCmdShow);
        WindowsApplication* GetWinApp();
        HWND GetWindowHandler();
        LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    }
}