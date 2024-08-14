#include "Components/RMaterialComponent.h"

namespace Faia
{
    namespace Root
    {
        void RMaterialComponent::AddMaterial(Material material)
        {
            std::shared_ptr<Material> materialPtr = std::make_shared<Material>(material);
            _materials.push_back(materialPtr);
        }

        Material* RMaterialComponent::GetMaterialOfIndex(int index)
        {
            if (index < _materials.size())
                return _materials[index].get();
            else
            {
                OutputDebugStringA("Error when trying to get material");
                return nullptr;
            }
        }
    }
}