#pragma once
#include "Graphics/Mesh/RMeshRenderer.h"

namespace Faia
{
    namespace Root
    {
        class RSkeletalMeshRenderer : public RMeshRenderer
        {
            typedef RMeshRenderer super;
        public:
            void ReadFromPath(const char* filePath) override;
        protected:
            void SetBuffers(RMesh& mesh, RMeshNode& rmeshNode) override;
        };
    }
}