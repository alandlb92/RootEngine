#include "Components/RMeshComponent.h"

//#include "Graphics/DX11/Mesh/RMeshRenderer_DX11.h"
//#include "Graphics/DX11/Mesh/RMesh_DX11.h"
#include "Core/ResourcesManager.h"

namespace Faia
{
    namespace Root
    {
        void RMeshComponent::LoadMesh(const char* meshDataPath)
        {
            //mRMeshRenderer = GetResourcesManager()->Load<RMeshRenderer_DX11>(meshDataPath);
        }

       /* std::vector<std::shared_ptr<RMesh_DX11>> RMeshComponent::GetMeshs()
        {
            return mRMeshRenderer->GetMeshs();
        }*/
    }
}