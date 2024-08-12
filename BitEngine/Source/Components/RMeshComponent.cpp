#include "Components/RMeshComponent.h"

#include "Graphics/Mesh/RMeshRenderer.h"
#include "Graphics/Mesh/RMesh.h"
#include "Core/ResourcesManager.h"

namespace Faia
{
    namespace Root
    {
        void RMeshComponent::LoadMesh(const char* meshDataPath)
        {
            mRMeshRenderer = GetResourcesManager()->Load<RMeshRenderer>(meshDataPath);
        }

        std::vector<RMesh> RMeshComponent::GetMeshs()
        {
            return mRMeshRenderer->GetMeshs();
        }
    }
}