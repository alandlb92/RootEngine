#include "Components/MaterialComponent.h"

namespace Faia
{
    namespace Root
    {
        void MaterialComponent::AddMaterial(Material material)
        {
            std::shared_ptr<Material> materialPtr = std::make_shared<Material>(material);
            _materials.push_back(materialPtr);
        }

        Material* MaterialComponent::GetMaterialOfIndex(int index)
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