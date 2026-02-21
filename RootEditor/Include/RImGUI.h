#pragma once

//Todo: Platform conditions
#include "imgui_impl_dx12.h"
#include "Graphics/RGraphics.h"

#include "imgui.h"
#include "imgui_impl_win32.h"


namespace Faia
{    
    namespace Editor
    {
        namespace UI
        {
            void ImGui_Impl_Init()
            {
                //Todo: DX12 exclusive
                Root::Graphics::Graphics_DX12* graphics = Root::Graphics::GetGraphics();
                ImGui_ImplDX12_Init(graphics->GetDevice(), Root::Graphics::SwapChainBufferCount, graphics->GetRtvFormat(),
                    graphics->GetCbvSrvHeap(), graphics->GetFontSrvCpuDescHandleForImGui(), graphics->GetFontSrvGpuDescHandleForImGui());
            }

            void ImGui_Impl_NewFrame()
            {
                ImGui_ImplDX12_NewFrame();
            }


            void ImGui_Impl_RenderDrawData()
            {
                Root::Graphics::Graphics_DX12* graphics = Root::Graphics::GetGraphics();
                ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), graphics->GetCommandList());
            }
        }
    }
}