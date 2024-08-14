#pragma once
#include "Scene.h"

namespace Faia
{
    namespace Root
    {
        class SceneManager
        {
            friend class RootEngine;

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
    }
}