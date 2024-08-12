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

        void RSkeletalMeshRenderer::SetBuffers(RMesh& mesh, RMeshNode& rmeshNode)
        {
            super::SetBuffers(mesh, rmeshNode);
            RSkeletalMesh& sm = static_cast<RSkeletalMesh&>(mesh);
            sm.SetBoneData(rmeshNode.mBoneData);
        }
    }
}