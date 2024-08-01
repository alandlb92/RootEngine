#pragma once
#include "FBXImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            class FBXMeshImporter : public FBXImporter
            {
                public:
                    void Run() override;
            };
        }
    }
}