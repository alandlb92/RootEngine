#include "REditor.h"
#include "REViewport.h"
#include "REInspector.h"
#include "Faia/WindowsApplication.h"
#include "Graphics/GraphicsMain.h"

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
                ImGui_ImplDX11_Init(Faia::Root::GetDevice(), Faia::Root::GetDeviceContext());
                Faia::Windows::GetWinApp()->RegisterWinAppProcFunction(std::bind(ImGui_ImplWin32_WndProcHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

                //float mWidthPercentage;
                //float mHeightPercentage;
                //float mPositionX;
                //float mPositionY;

                float windoWidth = Faia::Windows::GetWinApp()->GetWidth();
                float windowHeight = Faia::Windows::GetWinApp()->GetHeight();

                REConfiguration viewportConfig{ .6f, 1.f, 0, 0 };
                std::unique_ptr<REViewport> viewPort = std::make_unique<REViewport>(viewportConfig);

                REConfiguration inspectorConfig{ .4f, 1.f, windoWidth * .6f, 0 };
                std::unique_ptr<REInspector> inspector = std::make_unique<REInspector>(inspectorConfig);

                Faia::Root::ResizeViewport(viewPort->GetWidth(), viewPort->GetHeight());

                mWindows.push_back(std::move(viewPort));
                mWindows.push_back(std::move(inspector));

            }

            void REditor::Update()
            {
                ImGui_ImplDX11_NewFrame();
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