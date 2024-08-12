#include "Graphics/Mesh/RMeshRenderer.h"
#include "Graphics/Mesh/RMesh.h"
#include "Data/RMeshData.h"

namespace Faia
{
    namespace Root
    {
        void RMeshRenderer::ReadFromPath(const char* filePath)
        {
            ReadFromPath<RMesh>(filePath);
        }

        std::vector<RMesh> RMeshRenderer::GetMeshs()
        {
            return mRMeshs;
        }

        void RMeshRenderer::SetBuffers(RMesh& mesh, RMeshNode& rmeshNode)
        {
            mesh.SetIndices(rmeshNode.mIndices);
            mesh.SetVertices(rmeshNode.mVertices);
            mesh.SetUV(rmeshNode.mUV);
            mesh.SetNormals(rmeshNode.mNormals);
            mesh.SetMaterialIndex(rmeshNode.mMaterialIndex);
        }
    }
}