#pragma once
#include "Core/RComponent.h"

#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMeshRenderer;
        class RMesh;

        class RMeshComponent : public RComponent
        {
            typedef RComponent Super;            

        public:
            virtual void LoadMesh(const char* meshDataPath);
            std::vector<std::shared_ptr<RMesh>> GetMeshs();

        protected:
            std::shared_ptr<RMeshRenderer> mRMeshRenderer;

        };
    }
}