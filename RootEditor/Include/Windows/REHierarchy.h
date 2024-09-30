#pragma once
#include "REWindow.h"

namespace Faia
{
    namespace Root
    {
        class SceneObject;
    }

    namespace Editor
    {
        namespace UI
        {
            class REHierarchy : public REWindow
            {
            public:
                REHierarchy(REConfiguration config);
                void Update() override;

            private:
                Root::SceneObject* mSceneObjectSelected;
            };

        }
    }
}