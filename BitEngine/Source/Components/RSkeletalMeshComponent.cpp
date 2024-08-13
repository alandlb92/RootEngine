#include "Components/RSkeletalMeshComponent.h"
#include "Graphics/Mesh/RSkeletalMeshRenderer.h"
#include "Core/ResourcesManager.h"

namespace Faia
{
    namespace Root
    {
        void RSkeletalMeshComponent::LoadMesh(const char* meshDataPath)
        {
            mRMeshRenderer = GetResourcesManager()->Load<RSkeletalMeshRenderer>(meshDataPath);
        }
    }
}