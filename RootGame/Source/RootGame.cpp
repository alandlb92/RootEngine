// RootGame.cpp : Defines the entry point for the application.
//
#include "RootGame.h"
#include "Faia/WindowsApplication.h"
#include "Core/RootEngine.h"
#include "Core/ResourcesManager.h"
#include "FaiaInputSystem.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    Faia::Windows::WindowsApplication app(hInstance, IDS_APP_TITLE, IDC_RootGame, IDI_SMALL);
    if (app.InitInstance(nCmdShow))
    {
        Faia::InputSystem::FaiaInputSystem inputSystem;
        Faia::Root::RootEngine engine(app.GetWindowHandler());
        engine.Init();
        app.RegisterWinAppUpdateFunction(std::bind(&Faia::Root::RootEngine::Update, &engine));
        app.RegisterWinAppProcFunction(std::bind(&Faia::InputSystem::FaiaInputSystem::SendOSEvent, &inputSystem, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        return app.Run(nCmdShow);
    }

    return -1;
}
