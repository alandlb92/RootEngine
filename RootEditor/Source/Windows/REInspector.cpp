#include "Windows/REInspector.h"
#include "Graphics/DX11/GraphicsMain_DX11.h"

#include "imgui.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {
            REInspector::REInspector(REConfiguration config) : REWindow(config) {}

            void REInspector::Update()
            {
                ImGui::SetNextWindowPos(ImVec2(mPositionX, mPositionY));
                ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight));
                ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                ImGui::End();
            }
        }
    }
}