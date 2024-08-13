#pragma once
#include <vector>
#include "Graphics/Mesh/RSkeletalMesh.h"

namespace Faia
{
    namespace Root
    {
        class RMeshData;
        struct RMeshNode;

        class RMeshRenderer
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
            std::vector<std::shared_ptr<RMesh>> GetMeshs();
        protected:
            virtual void SetBuffers(std::shared_ptr<RMesh> mesh, RMeshNode& rmeshNode);
            std::vector<std::shared_ptr<RMesh>> mRMeshs;
        };
    }
}