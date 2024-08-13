#pragma once
#include "Components/RMeshComponent.h"

namespace Faia
{
    namespace Root
    {
        class RSkeletalMeshComponent : public RMeshComponent
        {
         public:
            void LoadMesh(const char* meshDataPath) override;
        };
    }
}