#include "Windows/REContent.h"
#include "REConfiguration.h"

#include "imgui.h"

Faia::Editor::UI::REContent::REContent(REConfiguration config) : REWindow(config)
{
}

void Faia::Editor::UI::REContent::Update()
{
    ImGui::SetNextWindowPos(ImVec2(mPositionX, mPositionY));
    ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight));
    ImGui::Begin("Content", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::End();
}
