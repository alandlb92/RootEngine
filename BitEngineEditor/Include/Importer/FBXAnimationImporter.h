#pragma once
#include "FBXImporter.h"


namespace Faia
{
    namespace BitEngineEditor
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