#include "Graphics/Mesh.h"
#include "Graphics/GraphicsMain.h"
#include <stdexcept>
#include "Data/BitMeshData.h"

namespace Faia
{
    namespace Root
    {
        std::vector<std::shared_ptr<Mesh>>  Mesh::MakeFromFbxFile(const char* filePath)
        {
            RMeshData bmd;
            std::vector<std::shared_ptr<Mesh>> meshs;
            bmd.ReadFromPath(filePath);
            for (auto& bmesh : bmd._meshs)
            {
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
                mesh->SetIndices(bmesh._indices);
                mesh->SetVertices(bmesh._vertices);
                mesh->SetUV(bmesh._uv);
                mesh->SetNormals(bmesh._normals);
                mesh->_materialIndex = bmesh._materialIndex;
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

        void Mesh::SetNormals(vector<Vector3D> normals)
        {
            _normals = normals;

            //Create and initialize the normals buffer
            D3D11_BUFFER_DESC normalBufferDesc;
            ZeroMemory(&normalBufferDesc, sizeof(D3D11_BUFFER_DESC));
            normalBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            normalBufferDesc.ByteWidth = sizeof(Vector3D) * _normals.size();
            normalBufferDesc.CPUAccessFlags = 0;
            normalBufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
            resourceData.pSysMem = _normals.data();
            HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&normalBufferDesc, &resourceData, _normalBuffer.GetAddressOf());

            if (FAILED(hr))
            {
                throw std::invalid_argument("Faile to create normal buffer");
            }
        }
    }
}