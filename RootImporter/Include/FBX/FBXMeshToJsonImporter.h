#pragma once
#include "RImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            class FBXMeshToJsonImporter : public RImporter
            {
            public:
                void Run() override;
            };
        }
    }
}