#pragma once

//Todo: Platform conditions
#include "imgui_impl_dx12.h"
#include "Graphics/RRenderSystem.h"

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
                Root::Graphics::GraphicsPlatform* graphics = static_cast<Root::Graphics::GraphicsPlatform*>(Root::Graphics::GetRenderSystem()->GetGraphicsPlatform());
               /* ImGui_ImplDX12_Init(graphics->GetDevice(), Root::Graphics::SwapChainBufferCount, graphics->GetRtvFormat(),
                    graphics->GetCbvSrvHeap(), graphics->GetFontSrvCpuDescHandleForImGui(), graphics->GetFontSrvGpuDescHandleForImGui());*/
            }

            void ImGui_Impl_NewFrame()
            {
                //ImGui_ImplDX12_NewFrame();
            }


            void ImGui_Impl_RenderDrawData()
            {
                Root::Graphics::GraphicsPlatform* graphics = static_cast<Root::Graphics::GraphicsPlatform*>(Root::Graphics::GetRenderSystem()->GetGraphicsPlatform());
                //ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), graphics->GetCommandList());
            }
        }
    }
}