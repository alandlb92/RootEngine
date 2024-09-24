#include "Faia/WindowsApplication.h"
#include "Core/RootEngine.h"
#include "FaiaInputSystem.h"
#include "Graphics/GraphicsMain.h"
#include "REditor.h"

#include "Resource.h"

// Forward declare message handler from imgui_impl_win32.cpp
using namespace Faia::Windows;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    InitializeApp(hInstance, IDS_APP_TITLE, IDC_RootEditor, IDI_SMALL, nCmdShow);
    GetWinApp()->RegisterWinAppUpdateFunction(std::bind(&Faia::Root::RootEngine::Update, Faia::Root::GetEngine()));
    GetWinApp()->RegisterWinAppProcFunction(std::bind(&Faia::InputSystem::FaiaInputSystem::SendOSEvent, Faia::InputSystem::GetFaiaInputSystem(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    Faia::Root::GetGraphics()->RegisterPostRendererFunction(std::bind(&Faia::Editor::UI::REditor::Update, Faia::Editor::UI::GetEditor()));
    return GetWinApp()->Run(nCmdShow);
}