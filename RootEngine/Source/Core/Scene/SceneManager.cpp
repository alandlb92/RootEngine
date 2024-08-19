#include "Core/Scene/SceneManager.h"
#include "Core/RComponent.h"

namespace Faia
{
    namespace Root
    {

        SceneManager* gSceneManager;

        SceneManager* GetSceneManager()
        {
            if (gSceneManager == nullptr)
            {
                gSceneManager = new SceneManager();
            }

            return gSceneManager;
        }

        SceneManager::SceneManager()
        {
            _currentScene = nullptr;
        }

        void SceneManager::Update(float deltaTime)
        {
            _currentScene->Update(deltaTime);
        }


        void SceneManager::LoadScene(Scene* scene)
        {
            _currentScene = scene;
            _currentScene->Init();
        }

        Scene* SceneManager::GetCurrentScene()
        {
            return _currentScene;
        }
    }
}
