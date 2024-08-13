#include "Graphics/Mesh/RSkeletalMeshRenderer.h"
#include "Graphics/Mesh/RSkeletalMesh.h"

namespace Faia
{
    namespace Root
    {
        void RSkeletalMeshRenderer::ReadFromPath(const char* filePath)
        {
            super::ReadFromPath<RSkeletalMesh>(filePath);
        }

        void RSkeletalMeshRenderer::SetBuffers(std::shared_ptr<RMesh> mesh, RMeshNode& rmeshNode)
        {
            super::SetBuffers(mesh, rmeshNode);
            std::shared_ptr<RSkeletalMesh> sm = std::dynamic_pointer_cast<RSkeletalMesh>(mesh);
            sm->SetBoneData(rmeshNode.mBoneData);
        }
    }
}