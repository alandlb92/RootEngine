#pragma once
#include "Graphics/DX11/Mesh/RMeshRenderer_DX11.h"

namespace Faia
{
    namespace Root
    {
        class RSkeletalMeshRenderer_DX11 : public RMeshRenderer_DX11
        {
            typedef RMeshRenderer_DX11 super;
        public:
            void ReadFromPath(const char* filePath) override;
        protected:
            void SetBuffers(std::shared_ptr<RMesh_DX11> mesh, RMeshNode& rmeshNode) override;
        };
    }
}