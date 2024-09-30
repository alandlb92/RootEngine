#pragma once
#include "SceneObject.h"
#include <list>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RCamera;
        class Scene : public RObject
        {
            friend class SceneManager;
            typedef RObject Super;

        public:            
            std::list<SceneObject*> GetRenderablebleObjects();
            std::list<SceneObject*> GetSceneObjects();
            void AddObject(SceneObject* obj);
            RCamera* GetMainCamera() { return mMainCamera; }

        protected:
            virtual void Init();
            virtual void Update(float deltaTime);

        private:
            std::list<SceneObject*> _sceneObjects;
            std::list<SceneObject*> _renderablebleObjects;

            void NotifyComponentAdded(RObject* obj, RComponent* component);
            RCamera* mMainCamera;
        };

        Scene* MakeDemoScene();
    }
}