#include "Components/RSkeletalMeshComponent.h"
//#include "Graphics/DX11/Mesh/RSkeletalMeshRenderer_DX11.h"
#include "Core/ResourcesManager.h"

namespace Faia
{
    namespace Root
    {
        void RSkeletalMeshComponent::LoadMesh(const char* meshDataPath)
        {
            //mRMeshRenderer = GetResourcesManager()->Load<RSkeletalMeshRenderer_DX11>(meshDataPath);
        }
    }
}