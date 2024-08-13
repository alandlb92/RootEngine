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
            void SetBuffers(std::shared_ptr<RMesh> mesh, RMeshNode& rmeshNode) override;
        };
    }
}