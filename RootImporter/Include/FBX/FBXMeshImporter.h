#pragma once
#include "RImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            class FBXMeshImporter : public RImporter
            {
                public:
                    void Run() override;
            };
        }
    }
}