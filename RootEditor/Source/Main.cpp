#include "Faia/WindowsApplication.h"
#include "Core/RootEngine.h"
#include "FaiaInputSystem.h"
#include "Graphics/GraphicsMain.h"

#include "Resource.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace Faia::Windows;
class Test
{
public:
    float viewportWidth;
    float viewportHeight;

    void init()
    {
        float leftWidth = GetWinApp()->GetWidth() * 0.6f;
        float rightWidth = GetWinApp()->GetWidth() - leftWidth;

        viewportWidth = GetWinApp()->GetWidth() * 0.6f;
        viewportHeight = GetWinApp()->GetHeight();

    }

    void Update()
    {
        // Iniciar frame do ImGui
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImVec2 windowSize = ImGui::GetIO().DisplaySize;

        float leftWidth = windowSize.x * 0.6f;
        float rightWidth = windowSize.x - leftWidth;

        viewportWidth = windowSize.x * 0.6f;
        viewportHeight = windowSize.y;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(viewportWidth, viewportHeight));

        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ID3D11ShaderResourceView* sceneTexture = Faia::Root::GetRenderOutSRV();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)sceneTexture, viewportSize);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(leftWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(rightWidth, windowSize.y));
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
};
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    Test test;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    InitializeApp(hInstance, IDS_APP_TITLE, IDC_RootEditor, IDI_SMALL, nCmdShow);
    GetWinApp()->RegisterWinAppUpdateFunction(std::bind(&Faia::Root::RootEngine::Update, Faia::Root::GetEngine()));
    GetWinApp()->RegisterWinAppProcFunction(std::bind(&Faia::InputSystem::FaiaInputSystem::SendOSEvent, Faia::InputSystem::GetFaiaInputSystem(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    GetWinApp()->RegisterWinAppProcFunction(std::bind(ImGui_ImplWin32_WndProcHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    Faia::Root::GraphicsMain* graphics = Faia::Root::GetGraphics();
    graphics->RegisterPostRendererFunction(std::bind(&Test::Update, &test));

    //// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    //// Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GetWindowHandler());
    ImGui_ImplDX11_Init(Faia::Root::GetDevice(), Faia::Root::GetDeviceContext());

    test.init();
    Faia::Root::ResizeViewport(test.viewportWidth, test.viewportHeight);    

    return GetWinApp()->Run(nCmdShow);
}