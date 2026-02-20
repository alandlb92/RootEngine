#include "Graphics/DX11/Mesh/RMeshRenderer_DX11.h"
#include "Graphics/DX11/Mesh/RMesh_DX11.h"
#include "Data/RMeshData.h"

namespace Faia
{
    namespace Root
    {
        void RMeshRenderer_DX11::ReadFromPath(const char* filePath)
        {
            ReadFromPath<RMesh_DX11>(filePath);
        }

        std::vector<std::shared_ptr<RMesh_DX11>> RMeshRenderer_DX11::GetMeshs()
        {
            return mRMeshs;
        }

        void RMeshRenderer_DX11::SetBuffers(std::shared_ptr<RMesh_DX11> mesh, RMeshNode& rmeshNode)
        {
            mesh->SetIndices(rmeshNode.mIndices);
            mesh->SetVertices(rmeshNode.mVertices);
            mesh->SetUV(rmeshNode.mUV);
            mesh->SetNormals(rmeshNode.mNormals);
            mesh->SetMaterialIndex(rmeshNode.mMaterialIndex);
        }
    }
}