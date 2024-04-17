#include "Core/Scene/SceneManager.h"
#include "Core/BComponent.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager()
{
    _instance = this;
}

void SceneManager::Init()
{
    _currentScene->Init();
}

void SceneManager::Update(float deltaTime)
{
    _currentScene->Update(deltaTime);
}


void SceneManager::LoadScene(Scene* scene)
{
    _currentScene = scene;
}

Scene* SceneManager::GetCurrentScene()
{
    return _currentScene;
}
