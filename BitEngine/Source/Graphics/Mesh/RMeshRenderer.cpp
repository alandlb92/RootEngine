#include "Graphics/Mesh/RMeshRenderer.h"
#include "Data/RMeshData.h"
#include "Graphics/Mesh/RMesh.h"

namespace Faia
{
    namespace Root
    {
        void RMeshRenderer::ReadFromPath(const char* filePath)
        {
            RMeshData bmd;
            bmd.ReadFromPath(filePath);
            for (auto& rmesh : bmd.mRMeshNodes)
            {                
                RMesh& mesh = mRMeshs.emplace_back();
                mesh.SetIndices(rmesh.mIndices);
                mesh.SetVertices(rmesh.mVertices);
                mesh.SetUV(rmesh.mUV);
                mesh.SetNormals(rmesh.mNormals);
                mesh.SetMaterialIndex(rmesh.mMaterialIndex);
            }
        }
        std::vector<RMesh> RMeshRenderer::GetMeshs()
        {
            return mRMeshs;
        }
    }
}