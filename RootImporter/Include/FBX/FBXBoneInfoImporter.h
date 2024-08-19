#pragma once
#include "FBXImporter.h"


namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            struct BoneNode
            {
                int boneId;
                int parent;
                std::vector<int> childs;
            };

            class FBXBoneInfoImporter : public FBXImporter
            {
            public:
                void Run() override;
            };
        }
    }
}