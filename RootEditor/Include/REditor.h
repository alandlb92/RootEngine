#pragma once
#include <vector>
#include <memory>

#include "REWindow.h"

namespace Faia
{
    namespace Editor
    {
        namespace UI
        {


            class REditor
            {
            public:
                void Init();
                void Update();

            private:
                std::vector<std::unique_ptr<REWindow>> mWindows;
            };

            REditor* GetEditor();
        }
    }
}

