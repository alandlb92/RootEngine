#pragma once
#include "Mesh.h"
#include "Data/BitMeshData.h"


class SkeletalMesh : public Mesh
{

public:
    static std::vector<std::shared_ptr<Mesh>>  MakeSkeletonMeshFromFbxFile(const char* filePath);

    ID3D11Buffer* GetBonesDataBuffer() { return _boneDataBuffer.Get(); }
    void SetBoneData(std::vector<RVertexBoneData> boneData);


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> _boneDataBuffer = nullptr;
    std::vector<RVertexBoneData> _boneData;

};