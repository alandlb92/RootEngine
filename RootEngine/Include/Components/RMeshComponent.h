#pragma once
#include "Core/RComponent.h"

#include <vector>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class RMeshRenderer_DX11;
        //class RMesh_DX11;

        class RMeshComponent : public RComponent
        {
            typedef RComponent Super;            

        public:
            virtual void LoadMesh(const char* meshDataPath);
            //std::vector<std::shared_ptr<RMesh_DX11>> GetMeshs();

        protected:
            //std::shared_ptr<RMeshRenderer_DX11> mRMeshRenderer;

        };
    }
}