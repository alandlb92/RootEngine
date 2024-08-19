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
            void LoadScene(Scene* scene);
            Scene* GetCurrentScene();

        private:
            void Update(float deltaTime);
            Scene* _currentScene;
        };

        SceneManager* GetSceneManager();
    }
}