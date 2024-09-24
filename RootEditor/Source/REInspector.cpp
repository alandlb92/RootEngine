#include "REInspector.h"
#include "imgui.h"
#include "Graphics/GraphicsMain.h"

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