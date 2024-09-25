#pragma once
#include "REWindow.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {
            class REHierarchy : public REWindow
            {
            public:
                REHierarchy(REConfiguration config);
                void Update() override;
            };

        }
    }
}