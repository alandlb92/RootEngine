#pragma once
#include "Graphics/DX11/Mesh/RMesh_DX11.h"
#include "Data/RMeshData.h"


namespace Faia
{
    namespace Root
    {
        class RSkeletalMesh_DX11 : public RMesh_DX11
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