#include "REViewport.h"
#include "imgui.h"
#include "Graphics/GraphicsMain.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {
            REViewport::REViewport(REConfiguration config) : REWindow(config)
            {

            }

            void REViewport::Update()
            {
                ImGui::SetNextWindowPos(ImVec2(mPositionX, mPositionY));
                ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight));
                ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                ID3D11ShaderResourceView* sceneTexture = Faia::Root::GetRenderOutSRV();
                ImVec2 viewportSize = ImGui::GetContentRegionAvail();
                ImGui::Image((void*)sceneTexture, viewportSize);
                ImGui::End();
            }
        }
    }
}