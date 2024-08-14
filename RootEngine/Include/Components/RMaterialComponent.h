#pragma once
#include "Core/RComponent.h"
#include "Graphics/RMaterial.h"
#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMaterialComponent : public RComponent
        {
        public:
            void AddMaterial(RMaterial material);
            RMaterial* GetMaterialOfIndex(int index);

        private:
            std::vector<std::shared_ptr<RMaterial>> _materials;

        };
    }
}