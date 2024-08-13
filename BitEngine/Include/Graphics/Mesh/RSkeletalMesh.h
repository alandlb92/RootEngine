#pragma once
#include "Graphics/Mesh/RMesh.h"
#include "Data/RMeshData.h"


namespace Faia
{
    namespace Root
    {
        class RSkeletalMesh : public RMesh
        {
        public:
            ID3D11Buffer* GetBonesDataBuffer() { return mBoneDataBuffer.Get(); }
            void SetBoneData(std::vector<RVertexBoneData> boneData);


        protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> mBoneDataBuffer = nullptr;
            std::vector<RVertexBoneData> _boneData;

        };
    }
}