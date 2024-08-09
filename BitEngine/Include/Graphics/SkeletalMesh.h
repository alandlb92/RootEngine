#pragma once
#include "Mesh/RMesh.h"
#include "Data/RMeshData.h"


namespace Faia
{
    namespace Root
    {
        class SkeletalMesh : public RMesh
        {

        public:
            static std::vector<std::shared_ptr<RMesh>>  MakeSkeletonMeshFromFbxFile(const char* filePath);

            ID3D11Buffer* GetBonesDataBuffer() { return _boneDataBuffer.Get(); }
            void SetBoneData(std::vector<Faia::Root::RVertexBoneData> boneData);


        protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> _boneDataBuffer = nullptr;
            std::vector<Faia::Root::RVertexBoneData> _boneData;

        };
    }
}