#pragma once
#include "SceneObject.h"
#include <list>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class Scene : public BObject
        {
            friend class SceneManager;
            typedef BObject Super;

        public:
            static Scene* MakeDemoScene();
            std::list<SceneObject*> GetRenderablebleObjects();
            void AddObject(SceneObject* obj);

        protected:
            virtual void Init();
            virtual void Update(float deltaTime);

        private:
            std::list<SceneObject*> _sceneObjects;
            std::list<SceneObject*> _renderablebleObjects;

            void NotifyComponentAdded(BObject* obj, BComponent* component);
        };
    }
}