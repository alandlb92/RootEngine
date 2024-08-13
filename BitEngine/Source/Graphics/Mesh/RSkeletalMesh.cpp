#include "Graphics/Mesh/RSkeletalMesh.h"
#include "Graphics/GraphicsMain.h"

namespace Faia
{
    namespace Root
    {
        void RSkeletalMesh::SetBoneData(std::vector<RVertexBoneData> boneData)
        {
            mMeshType = RMeshType::Skeletal;

            _boneData = boneData;

            //Create and initialize the normals buffer
            D3D11_BUFFER_DESC boneIdsBufferDesc;
            ZeroMemory(&boneIdsBufferDesc, sizeof(D3D11_BUFFER_DESC));
            boneIdsBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            boneIdsBufferDesc.ByteWidth = sizeof(RVertexBoneData) * _boneData.size();
            boneIdsBufferDesc.CPUAccessFlags = 0;
            boneIdsBufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
            resourceData.pSysMem = _boneData.data();
            HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&boneIdsBufferDesc, &resourceData, mBoneDataBuffer.GetAddressOf());

            if (FAILED(hr))
            {
                throw std::invalid_argument("Faile to create BoneData buffer");
            }
        }
    }
}