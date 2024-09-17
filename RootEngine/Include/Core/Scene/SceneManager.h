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


            template<typename T>
            T* GetComponentOfType()
            {
                for (auto& sceneObj : _currentScene->_sceneObjects)
                {
                    if (T* comp = sceneObj->GetComponentOfType<T>())
                    {
                        return comp;
                    }
                }

                return nullptr;
            }

        private:
            void Update(float deltaTime);
            Scene* _currentScene;
        };

        SceneManager* GetSceneManager();
    }
}