#pragma once
#include "RImporter.h"


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

            class FBXBoneInfoImporter : public RImporter
            {
            public:
                void Run() override;
            };
        }
    }
}