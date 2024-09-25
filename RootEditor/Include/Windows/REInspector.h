#pragma once
#include "REWindow.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {
            class REInspector : public REWindow
            {
            public:
                REInspector(REConfiguration config);
                void Update() override;
            };

        }
    }
}