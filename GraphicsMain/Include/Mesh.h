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

    ID3D11Buffer* _vertexBuffer = nullptr;
    ID3D11Buffer* _indexBuffer = nullptr;

    vector<uint16_t> _indices;
private:
    vector<Vector3D> _vertices;

};