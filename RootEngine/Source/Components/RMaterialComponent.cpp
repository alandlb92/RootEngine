#include "Components/RMaterialComponent.h"

namespace Faia
{
    namespace Root
    {
        void RMaterialComponent::AddMaterial(RMaterial material)
        {
            std::shared_ptr<RMaterial> materialPtr = std::make_shared<RMaterial>(material);
            _materials.push_back(materialPtr);
        }

        RMaterial* RMaterialComponent::GetMaterialOfIndex(int index)
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