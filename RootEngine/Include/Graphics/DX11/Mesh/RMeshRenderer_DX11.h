#pragma once
#include <vector>
#include "Graphics/DX11/Mesh/RSkeletalMesh_DX11.h"

namespace Faia
{
    namespace Root
    {
        class RMeshData;
        struct RMeshNode;

        class RMeshRenderer_DX11
        {
        public:

            template<typename Mesh>
            void ReadFromPath(const char* filePath)
            {
                RMeshData bmd;
                bmd.ReadFromPath(filePath);
                for (RMeshNode& rmeshNode : bmd.mRMeshNodes)
                {
                    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
                    SetBuffers(mesh, rmeshNode);
                    mRMeshs.push_back(mesh);
                }
            }

            virtual void ReadFromPath(const char* filePath);
            std::vector<std::shared_ptr<RMesh_DX11>> GetMeshs();
        protected:
            virtual void SetBuffers(std::shared_ptr<RMesh_DX11> mesh, RMeshNode& rmeshNode);
            std::vector<std::shared_ptr<RMesh_DX11>> mRMeshs;
        };
    }
}