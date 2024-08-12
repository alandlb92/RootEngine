#pragma once
#include <vector>

namespace Faia
{
    namespace Root
    {
        class RMeshData;
        class RMesh;
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
                    Mesh mesh;
                    SetBuffers(mesh, rmeshNode);
                    mRMeshs.push_back(mesh);
                }
            }

            virtual void ReadFromPath(const char* filePath);
            std::vector<RMesh> GetMeshs();
        protected:
            virtual void SetBuffers(RMesh& mesh, RMeshNode& rmeshNode);
            std::vector<RMesh> mRMeshs;
        };
    }
}