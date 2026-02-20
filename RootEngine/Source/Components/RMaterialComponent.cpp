#include "Components/RMaterialComponent.h"

namespace Faia
{
    namespace Root
    {
        void RMaterialComponent::AddMaterial(RMaterial_DX11 material)
        {
            std::shared_ptr<RMaterial_DX11> materialPtr = std::make_shared<RMaterial_DX11>(material);
            _materials.push_back(materialPtr);
        }

        RMaterial_DX11* RMaterialComponent::GetMaterialOfIndex(int index)
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