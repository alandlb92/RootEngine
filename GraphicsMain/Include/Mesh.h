#pragma once
#include <vector>
#include "Vector3D.h"
#include "d3d11.h"

using namespace std;

class Mesh
{
public:
    static Mesh MakePrimitiveCube();
    void SetVertices(vector<Vector3D> verctices);
    void SetIndices(vector<uint16_t> indices);

    ID3D11Buffer* GetVertexBuffer() { return _vertexBuffer; }
    ID3D11Buffer* GetIndexBuffer() { return _indexBuffer; }
    vector<Vector3D> GetVertices() { return _vertices; }
    vector<uint16_t> GetIndices() { return _indices; }
    size_t GetIndicesSize() { return _indices.size(); }

private:
    ID3D11Buffer* _vertexBuffer = nullptr;
    ID3D11Buffer* _indexBuffer = nullptr;
    vector<uint16_t> _indices;
    vector<Vector3D> _vertices;

};