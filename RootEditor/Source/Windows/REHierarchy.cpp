#include "Windows/REHierarchy.h"
#include "REConfiguration.h"

//todo: we need to put this in a controller if the code will upsize
#include "Core/Scene/SceneManager.h"

#include "imgui.h"

using namespace Faia::Root;

Faia::Editor::UI::REHierarchy::REHierarchy(REConfiguration config) : REWindow(config)
{
}

void Faia::Editor::UI::REHierarchy::Update()
{
    ImGui::SetNextWindowPos(ImVec2(mPositionX, mPositionY));
    ImGui::SetNextWindowSize(ImVec2(mWidth, mHeight));
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    SceneManager* sceneManager = GetSceneManager();
    std::list<SceneObject*> sceneObjects = sceneManager->GetCurrentScene()->GetSceneObjects();

    for (SceneObject* scObj : sceneObjects)
    {
        if (ImGui::Selectable(scObj->GetName().c_str(), scObj == mSceneObjectSelected))
        {
            mSceneObjectSelected = scObj;
        }
    }

    ImGui::End();
}

