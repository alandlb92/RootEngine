#pragma once
#include <vector>
namespace Faia
{
    namespace Root
    {
        class RMesh;

        class RMeshRenderer
        {
        public:
            void ReadFromPath(const char* filePath);
            std::vector<RMesh> GetMeshs();

        private:
            std::vector<RMesh> mRMeshs;
        };
    }
}