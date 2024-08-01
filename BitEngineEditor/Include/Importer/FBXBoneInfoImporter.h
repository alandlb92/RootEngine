#pragma once
#include "FBXImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            class FBXBoneInfoImporter : public FBXImporter
            {
            public:
                void Run() override;
            };
        }
    }
}