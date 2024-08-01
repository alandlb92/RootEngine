#include "Core/WindowsApplication.h"
#include "Core/BitEngine.h"


namespace Faia
{
    WindowsApplication* WindowsApplication::_appInstance = nullptr;

    WindowsApplication::WindowsApplication(HINSTANCE hInstance, UINT idsAppTittle, UINT idcGame, UINT idcSamall)
        : hInstance(hInstance), windowHandler(nullptr) {
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

    int WindowsApplication::Run(int nCmdShow) {
        if (!MyRegisterClass()) {
            return FALSE;
        }

        if (!InitInstance(nCmdShow)) {
            return FALSE;
        }

        HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(_idcGame));

        MSG msg = { 0 };
        std::unique_ptr<Root::BitEngine> engine = std::make_unique<Root::BitEngine>(windowHandler);
        _inputSystem = std::make_unique<Faia::InputSystem::FaiaInputSystem>();
        engine->Init();

        while (msg.message != WM_QUIT) {
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            engine->Update();
        }

        return (int)msg.wParam;
    }

    ATOM WindowsApplication::MyRegisterClass() {
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
        windowHandler = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

        if (!windowHandler) {
            return FALSE;
        }

        ShowWindow(windowHandler, nCmdShow);
        UpdateWindow(windowHandler);

        return TRUE;
    }

    LRESULT CALLBACK WindowsApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_CREATE) {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }

        if (_appInstance)
        {
            if (_appInstance->_inputSystem)
                _appInstance->_inputSystem->SendOSEvent(message, wParam, lParam);
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}