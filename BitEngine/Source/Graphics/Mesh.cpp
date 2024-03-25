#include "Graphics/Mesh.h"
#include "Graphics/GraphicsMain.h"
#include <stdexcept>
#include "Data/BitMeshData.h"

Mesh Mesh::MakePrimitiveCube()
{

    Mesh mesh;

    float side = 1.0f / 2.0f;
    std::vector<Vector3D> vertices(14, Vector3D(0));
    vertices[0] = Vector3D(-side, -side, -side);
    vertices[1] = Vector3D(side, -side, -side);
    vertices[2] = Vector3D(-side, side, -side);
    vertices[3] = Vector3D(side, side, -side);
    vertices[4] = Vector3D(-side, -side, side);
    vertices[5] = Vector3D(side, -side, side);
    vertices[6] = Vector3D(-side, side, side);
    vertices[7] = Vector3D(side, side, side);
    vertices[8] = Vector3D(-side, -side, -side);
    vertices[9] = Vector3D(side, -side, -side);
    vertices[10] = Vector3D(-side, -side, -side);
    vertices[11] = Vector3D(-side, -side, side);
    vertices[12] = Vector3D(side, -side, -side);
    vertices[13] = Vector3D(side, -side, side);

    vector<uint16_t> indices =
    {
        0,2,1,   2,3,1,
        4,8,5,   5,8,9,
        2,6,3,   3,6,7,
        4,5,7,   4,7,6,
        2,10,11, 2,11,6,
        12,3,7,  12,7,13
    };


    vector<Vector2D> uvs(14, Vector2D(0));;
    uvs[0] = { 2.0f / 3.0f,0.0f / 4.0f };
    uvs[1] = { 1.0f / 3.0f,0.0f / 4.0f };
    uvs[2] = { 2.0f / 3.0f,1.0f / 4.0f };
    uvs[3] = { 1.0f / 3.0f,1.0f / 4.0f };
    uvs[4] = { 2.0f / 3.0f,3.0f / 4.0f };
    uvs[5] = { 1.0f / 3.0f,3.0f / 4.0f };
    uvs[6] = { 2.0f / 3.0f,2.0f / 4.0f };
    uvs[7] = { 1.0f / 3.0f,2.0f / 4.0f };
    uvs[8] = { 2.0f / 3.0f,4.0f / 4.0f };
    uvs[9] = { 1.0f / 3.0f,4.0f / 4.0f };
    uvs[10] = { 3.0f / 3.0f,1.0f / 4.0f };
    uvs[11] = { 3.0f / 3.0f,2.0f / 4.0f };
    uvs[12] = { 0.0f / 3.0f,1.0f / 4.0f };
    uvs[13] = { 0.0f / 3.0f,2.0f / 4.0f };

    for (auto& uv : uvs)
    {
        uv = uv * 2.0f;
    }


    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
    mesh.SetUV(uvs);

    return mesh;
}

std::vector<Mesh> Mesh::MakeFromFbxFile(const char* filePath)
{
    BitMeshData bmd;
    std::vector<Mesh> meshs;
    bmd.ReadFromPath(filePath);
    for (auto& bmesh: bmd._meshs)
    {
        Mesh mesh;
        mesh.SetIndices(bmesh._indices);
        mesh.SetVertices(bmesh._vertices);
        mesh.SetUV(bmesh._uv);
        mesh._materialIndex = bmesh._materialIndex;
        meshs.push_back(mesh);
    }    

    return meshs;
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
    HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, _vertexBuffer.GetAddressOf());

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
    indexBufferDesc.ByteWidth = sizeof(uint16_t) * _indices.size();
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    resourceData.pSysMem = _indices.data();

    auto* device = GraphicsMain::GetDevice();
    HRESULT hr = device->CreateBuffer(&indexBufferDesc, &resourceData, _indexBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        throw std::invalid_argument("Faile to create index buffer");
    }
}

void Mesh::SetUV(vector<Vector2D> uv)
{
    _uv = uv;
    // Create and initialize the index buffer.
    D3D11_BUFFER_DESC uvBufferDesc;
    ZeroMemory(&uvBufferDesc, sizeof(D3D11_BUFFER_DESC));
    uvBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    uvBufferDesc.ByteWidth = sizeof(Vector2D) * _uv.size();
    uvBufferDesc.CPUAccessFlags = 0;
    uvBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    resourceData.pSysMem = _uv.data();

    HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&uvBufferDesc, &resourceData, _uvBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::invalid_argument("Faile to create uv buffer");
    }
}