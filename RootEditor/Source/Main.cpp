#include "Faia/WindowsApplication.h"
#include "Core/RootEngine.h"
#include "FaiaInputSystem.h"
#include "Graphics/GraphicsMain.h"

#include "Resource.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

using namespace Faia::Windows;
class Test
{
public:
    void Update()
    {
        // Iniciar frame do ImGui
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Renderizar a parte de interface do ImGui
        ImGui::Begin("Editor UI");
        ImGui::Text("Controles do Editor");

        // Colocar outros componentes de UI aqui (botões, sliders, etc.)
        ImGui::End();

        // Renderizar a cena 3D no viewport
        //ImGui::Begin("3D Viewport");

        //// Obter a textura renderizada da sua engine
        //ID3D11ShaderResourceView* sceneTexture = Faia::Root::GetSceneRenderTargetSRV();

        //// Desenhar a textura no ImGui
        //ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        //ImGui::Image((void*)sceneTexture, viewportSize);

        //ImGui::End();

        // Renderizar o ImGui
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

    Faia::Root::GetGraphics()->RegisterPostRendererFunction(std::bind(&Test::Update, &test));

    //// Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //// Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ////ImGui::StyleColorsLight();
    
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GetWindowHandler());
    ImGui_ImplDX11_Init(Faia::Root::GetDevice(), Faia::Root::GetDeviceContext());
    


  
    return GetWinApp()->Run(nCmdShow);
}