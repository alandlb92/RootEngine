#include "Graphics/SkeletalMesh.h"
#include "Graphics/GraphicsMain.h"
#include <stdexcept>


std::vector<std::shared_ptr<Mesh>>  SkeletalMesh::MakeSkeletonMeshFromFbxFile(const char* filePath)
{
    //todo: bmd need to be a pointer in a manager to apply flyweight
    RMeshData bmd;
    std::vector<std::shared_ptr<Mesh>>  meshs;
    bmd.ReadFromPath(filePath);
    for (auto& bmesh : bmd._meshs)
    {
        shared_ptr<SkeletalMesh> mesh = std::make_shared<SkeletalMesh>();
        mesh->SetIndices(bmesh._indices);
        mesh->SetVertices(bmesh._vertices);
        mesh->SetUV(bmesh._uv);
        mesh->SetNormals(bmesh._normals);
        mesh->SetBoneData(bmesh._boneData);
        mesh->_materialIndex = bmesh._materialIndex;
        meshs.push_back(mesh);
    }

    return meshs;
}


void SkeletalMesh::SetBoneData(std::vector<RVertexBoneData> boneData)
{
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
    HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&boneIdsBufferDesc, &resourceData, _boneDataBuffer.GetAddressOf());

    if (FAILED(hr))
    {
        throw std::invalid_argument("Faile to create BoneData buffer");
    }
}