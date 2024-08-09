#pragma once
#include "Graphics/Mesh/RMeshRenderer.h"
#include "Core/BComponent.h"

#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMesh;

        class RMeshComponent : public BComponent
        {
            typedef BComponent Super;            

        public:
            void LoadMesh(const char* meshDataPath);
            std::vector<RMesh> GetMeshs();

        private:
            std::shared_ptr<RMeshRenderer> mRMeshRenderer;

        };
    }
}