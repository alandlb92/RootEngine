#include "WindowsApplication.h"


namespace Faia
{
    namespace Windows
    {
        HWND gWindowHandler;
        HWND GetWindowHandler()
        {
            return gWindowHandler;
        }

        WindowsApplication* WindowsApplication::_appInstance = nullptr;

        WindowsApplication::WindowsApplication(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall)
            : hInstance(hInstance) {
            _idcSamall = idcSamall;
            _idsAppTittle = idsAppTittle;
            _idcGame = idcGame;
            LoadStringW(hInstance, _idsAppTittle, szTitle, MAX_LOADSTRING);
            LoadStringW(hInstance, _idcGame, szWindowClass, MAX_LOADSTRING);
            _appInstance = this;
        }

        WindowsApplication::~WindowsApplication() {
            _appInstance = nullptr;
        }


        void  WindowsApplication::RegisterWinAppProcFunction(NotifyWinProc winprocFunction)
        {
            mFunctionsNotifyAppWinProc.push_back(winprocFunction);
        }

        void  WindowsApplication::RegisterWinAppUpdateFunction(NotifyUpdate appUpdateFunction)
        {
            mFunctionsNotifyAppUpdate.push_back(appUpdateFunction);
        }

        int WindowsApplication::Run(int nCmdShow) {
            HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(_idcGame));

            MSG msg = { 0 };

            while (msg.message != WM_QUIT) {
                if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                for (NotifyUpdate& updateFunc : mFunctionsNotifyAppUpdate)
                {
                    updateFunc();
                }
            }

            return (int)msg.wParam;
        }

        ATOM WindowsApplication::RegisterClass() {
            WNDCLASSEXW wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);

            wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wcex.lpfnWndProc = WndProc;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = hInstance;
            wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(_idcGame));
            wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName = MAKEINTRESOURCEW(_idcGame);
            wcex.lpszClassName = szWindowClass;
            wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(_idcSamall));

            return RegisterClassExW(&wcex);
        }

        BOOL WindowsApplication::InitInstance(int nCmdShow) {
            if (!RegisterClass()) {
                return FALSE;
            }

            gWindowHandler = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

            if (!gWindowHandler) {
                return FALSE;
            }

            ShowWindow(gWindowHandler, nCmdShow);
            UpdateWindow(gWindowHandler);

            return TRUE;
        }

        LRESULT CALLBACK WindowsApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
            if (message == WM_CREATE) {
                LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            }

            if (_appInstance)
            {
                for (NotifyWinProc& updateFunc : _appInstance->mFunctionsNotifyAppWinProc)
                {
                    updateFunc(hWnd, message, wParam, lParam);
                }
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
}