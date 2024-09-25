#pragma once
#include "REWindow.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {
            class REContent : public REWindow
            {
            public:
                REContent(REConfiguration config);
                void Update() override;
            };

        }
    }
}