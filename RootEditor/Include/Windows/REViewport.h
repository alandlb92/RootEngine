#pragma once
#include "REWindow.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {
            class REViewport : public REWindow
            {
            public:
                REViewport(REConfiguration config);
                void Update() override;
            };

        }
    }
}