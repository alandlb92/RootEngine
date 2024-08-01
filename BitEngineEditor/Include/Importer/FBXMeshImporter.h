#pragma once
#include "FBXImporter.h"


namespace Faia
{
    namespace BitEngineEditor
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