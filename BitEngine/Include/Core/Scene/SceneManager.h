#pragma once
#include "Scene.h"

class SceneManager
{
    friend class BitEngine;

public:
    SceneManager();
    static SceneManager* GetInstance() { return _instance; };
    void LoadScene(Scene* scene);
    Scene* GetCurrentScene();

private:
    static SceneManager* _instance;

    void Init();
    void Update(float deltaTime);
    Scene* _currentScene;
};