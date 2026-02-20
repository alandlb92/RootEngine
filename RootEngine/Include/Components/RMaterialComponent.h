#pragma once
#include "Core/RComponent.h"
#include "Graphics/DX11/RMaterial_DX11.h"
#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMaterialComponent : public RComponent
        {
        public:
            void AddMaterial(RMaterial_DX11 material);
            RMaterial_DX11* GetMaterialOfIndex(int index);

        private:
            std::vector<std::shared_ptr<RMaterial_DX11>> _materials;

        };
    }
}