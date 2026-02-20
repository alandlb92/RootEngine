#include "Graphics/DX11/Mesh/RSkeletalMeshRenderer_DX11.h"
#include "Graphics/DX11/Mesh/RSkeletalMesh_DX11.h"

namespace Faia
{
    namespace Root
    {
        void RSkeletalMeshRenderer_DX11::ReadFromPath(const char* filePath)
        {
            super::ReadFromPath<RSkeletalMesh_DX11>(filePath);
        }

        void RSkeletalMeshRenderer_DX11::SetBuffers(std::shared_ptr<RMesh_DX11> mesh, RMeshNode& rmeshNode)
        {
            super::SetBuffers(mesh, rmeshNode);
            std::shared_ptr<RSkeletalMesh_DX11> sm = std::dynamic_pointer_cast<RSkeletalMesh_DX11>(mesh);
            sm->SetBoneData(rmeshNode.mBoneData);
        }
    }
}