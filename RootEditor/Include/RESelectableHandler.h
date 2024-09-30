#pragma once
#include "Core/Scene/SceneManager.h"
#include <memory>

namespace Faia
{
    namespace Editor
    {
        class RESelectableProvider
        {
        protected:
            void SelectObject(Root::SceneObject* obj);
            Root::SceneObject* GetSelectedObject();            
        };

        class RESelectableHandler
        {
        public:
            void SetSelectedObject(Root::SceneObject* obj);
            Root::SceneObject* GetSelectedObject();

        private:
            Root::SceneObject* mSelectedObject;
        };

    }
}

