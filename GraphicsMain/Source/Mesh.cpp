#include "pch.h"
#include "Mesh.h"
#include "GraphicsMain.h"
#include <stdexcept>

Mesh Mesh::MakePrimitiveCube()
{
    Mesh mesh;

    vector<Vector3D> vertices =
    {
        Vector3D(-1.0f, -1.0f, -1.0f), // 0
        Vector3D(-1.0f,  1.0f, -1.0f), // 1
        Vector3D(1.0f,  1.0f, -1.0f),  // 2
        Vector3D(1.0f, -1.0f, -1.0f),  // 3
        Vector3D(-1.0f, -1.0f,  1.0f), // 4
        Vector3D(-1.0f,  1.0f,  1.0f), // 5
        Vector3D(1.0f,  1.0f,  1.0f),  // 6
        Vector3D(1.0f, -1.0f,  1.0f)   // 7
    };

    vector<uint16_t> indices =
    {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        4, 5, 1, 4, 1, 0,
        3, 2, 6, 3, 6, 7,
        1, 5, 6, 1, 6, 2,
        4, 0, 3, 4, 3, 7
    };
    
    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);

    return mesh;
}

void Mesh::SetVertices(vector<Vector3D> verctices)
{
    _vertices = verctices;

    //Create and initialize the vertex buffer
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = sizeof(Vector3D) * _vertices.size();
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    resourceData.pSysMem = _vertices.data();
    HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, &_vertexBuffer);
    
    if (FAILED(hr))
    {
        throw std::invalid_argument("Faile to create vertex buffer");
    }
}

void Mesh::SetIndices(vector<uint16_t> indices)
{
    _indices = indices;

    // Create and initialize the index buffer.
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof(uint32_t) * _indices.size();
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    resourceData.pSysMem = _indices.data();

    HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&indexBufferDesc, &resourceData, &_indexBuffer);
    if (FAILED(hr))
    {
        throw std::invalid_argument("Faile to create index buffer");
    }
}
