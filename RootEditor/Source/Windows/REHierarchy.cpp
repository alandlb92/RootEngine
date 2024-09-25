#include "Windows/REHierarchy.h"
#include "REConfiguration.h"

#include "Core/Scene/SceneManager.h"

#include "imgui.h"

Faia::Editor::UI::REHierarchy::REHierarchy(REConfiguration config) : REWindow(config)
{
}

void Faia::Editor::UI::REHierarchy::Update()
{
    ImGui::SetNextWindowPos(ImVec2(mPositionX, mPositionY));
    ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight));
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    

    ImGui::End();
}

