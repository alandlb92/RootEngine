#pragma once
#include "Core/BComponent.h"

#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMeshRenderer;
        class RMesh;

        class RMeshComponent : public BComponent
        {
            typedef BComponent Super;            

        public:
            virtual void LoadMesh(const char* meshDataPath);
            std::vector<RMesh> GetMeshs();

        protected:
            std::shared_ptr<RMeshRenderer> mRMeshRenderer;

        };
    }
}