#pragma once
#include <vector>
#include "Data/Vector3D.h"
#include "d3d11.h"
#include <memory>
#include <wrl/client.h>
#include "Core/BComponent.h"

using namespace std;

class Mesh
{
public:
    static std::vector<std::shared_ptr<Mesh>>  MakeFromFbxFile(const char* filePath);

    virtual ~Mesh() = default;

    void SetVertices(vector<Vector3D> verctices);
    void SetIndices(vector<uint16_t> indices);
    void SetUV(vector<Vector2D> uv);
    void SetNormals(vector<Vector3D> normals);

    ID3D11Buffer* GetVertexBuffer() { return _vertexBuffer.Get(); }
    ID3D11Buffer* GetIndexBuffer() { return _indexBuffer.Get(); }
    ID3D11Buffer* GetUvBuffer() { return _uvBuffer.Get(); }
    ID3D11Buffer* GetNormalBuffer() { return _normalBuffer.Get(); }

    vector<Vector3D> GetVertices() { return _vertices; }
    vector<Vector3D> GetNormals() { return _normals; }
    vector<uint16_t> GetIndices() { return _indices; }
    size_t GetIndicesSize() { return _indices.size(); }
    uint16_t GetMaterialIndex() { return _materialIndex; }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _uvBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _normalBuffer = nullptr;
    
    vector<uint16_t> _indices;
    vector<Vector3D> _vertices;
    vector<Vector2D> _uv;
    vector<Vector3D> _normals;
    uint16_t _materialIndex;

};