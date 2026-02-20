#include "REditor.h"
#include "Faia/WindowsApplication.h"
#include "Graphics/DX11/GraphicsMain_DX11.h"

#include "Windows/REInspector.h"
#include "Windows/REViewport.h"
#include "Windows/REContent.h"
#include "Windows/REHierarchy.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {

            REditor* gEditor;
            REditor* GetEditor()
            {
                if (gEditor == nullptr)
                {
                    gEditor = new REditor();
                    gEditor->Init();
                }

                return gEditor;
            }

            void REditor::Init()
            {
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
                ImGui_ImplWin32_Init(Faia::Windows::GetWindowHandler());
                //This must be something multplatform
                //ImGui_ImplDX11_Init(Faia::Root::GetDevice(), Faia::Root::GetDeviceContext());
                Faia::Windows::GetWinApp()->RegisterWinAppProcFunction(std::bind(ImGui_ImplWin32_WndProcHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

                float windowWidth = Faia::Windows::GetWinApp()->GetWidth();
                float windowHeight = Faia::Windows::GetWinApp()->GetHeight();


                REConfiguration hierarchyConfig{ .2f, .7f, 0, 0 };
                std::unique_ptr<REHierarchy> hierarchy = std::make_unique<REHierarchy>(hierarchyConfig);

                REConfiguration viewportConfig{ .6f, .7f, .2f * windowWidth, 0 };
                std::unique_ptr<REViewport> viewPort = std::make_unique<REViewport>(viewportConfig);

                REConfiguration inspectorConfig{ .4f, .7f, windowWidth * .8f, 0 };
                std::unique_ptr<REInspector> inspector = std::make_unique<REInspector>(inspectorConfig);

                REConfiguration contentConfig{ 1.f, .7f, 0, 0.7f * windowHeight };
                std::unique_ptr<REContent> content = std::make_unique<REContent>(contentConfig);
                //TODO: must be multplatform
                //Faia::Root::ResizeViewport(viewPort->GetWidth(), viewPort->GetHeight());

                mWindows.push_back(std::move(viewPort));
                mWindows.push_back(std::move(inspector));
                mWindows.push_back(std::move(hierarchy));
                mWindows.push_back(std::move(content));

            }

            void REditor::Update()
            {
                //TODO: must be multplatform
                //ImGui_ImplDX11_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                for (std::unique_ptr<REWindow>& window : mWindows)
                {
                    window->Update();
                }

                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            }
        }
    }
}