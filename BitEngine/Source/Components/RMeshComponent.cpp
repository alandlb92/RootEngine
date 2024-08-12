#include "Components/RMeshComponent.h"
#include "FaiaInputSystem.h"
#include "Core/ResourcesManager.h"
#include "Graphics/Mesh/RMesh.h"

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