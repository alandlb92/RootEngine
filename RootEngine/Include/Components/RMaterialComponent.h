#pragma once
#include "Core/RComponent.h"
#include "Graphics/Material.h"
#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMaterialComponent : public RComponent
        {
        public:
            void AddMaterial(Material material);
            Material* GetMaterialOfIndex(int index);

        private:
            std::vector<std::shared_ptr<Material>> _materials;

        };
    }
}