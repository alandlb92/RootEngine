#pragma once
#include "FBXImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            class FBXAnimationImporter : public FBXImporter
            {
            public:
                void Run() override;
            };
        }
    }
}