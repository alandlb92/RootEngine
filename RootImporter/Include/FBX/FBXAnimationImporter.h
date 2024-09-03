#pragma once
#include "RImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            class FBXAnimationImporter : public RImporter
            {
            public:
                void Run() override;
            };
        }
    }
}