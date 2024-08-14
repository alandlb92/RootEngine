#include "Graphics/Mesh/RMesh.h"
#include "Graphics/GraphicsMain.h"
#include <stdexcept>
#include "Data/RMeshData.h"

namespace Faia
{
    namespace Root
    {
        void RMesh::SetVertices(vector<RVector3D> verctices)
        {
            mVertices = verctices;

            //Create and initialize the vertex buffer
            D3D11_BUFFER_DESC vertexBufferDesc;
            ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.ByteWidth = sizeof(RVector3D) * mVertices.size();
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
            resourceData.pSysMem = mVertices.data();
            HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, mVertexBuffer.GetAddressOf());

            if (FAILED(hr))
            {
                throw std::invalid_argument("Faile to create vertex buffer");
            }
        }

        void RMesh::SetIndices(vector<uint16_t> indices)
        {
            mIndices = indices;

            // Create and initialize the index buffer.
            D3D11_BUFFER_DESC indexBufferDesc;
            ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.ByteWidth = sizeof(uint16_t) * mIndices.size();
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
            resourceData.pSysMem = mIndices.data();

            auto* device = GraphicsMain::GetDevice();
            HRESULT hr = device->CreateBuffer(&indexBufferDesc, &resourceData, mIndexBuffer.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                throw std::invalid_argument("Faile to create index buffer");
            }
        }

        void RMesh::SetUV(vector<RVector2D> uv)
        {
            mUv = uv;
            // Create and initialize the index buffer.
            D3D11_BUFFER_DESC uvBufferDesc;
            ZeroMemory(&uvBufferDesc, sizeof(D3D11_BUFFER_DESC));
            uvBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            uvBufferDesc.ByteWidth = sizeof(RVector2D) * mUv.size();
            uvBufferDesc.CPUAccessFlags = 0;
            uvBufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
            resourceData.pSysMem = mUv.data();

            HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&uvBufferDesc, &resourceData, mUvBuffer.GetAddressOf());
            if (FAILED(hr))
            {
                throw std::invalid_argument("Faile to create uv buffer");
            }
        }

        void RMesh::SetNormals(vector<RVector3D> normals)
        {
            mNormals = normals;

            //Create and initialize the normals buffer
            D3D11_BUFFER_DESC normalBufferDesc;
            ZeroMemory(&normalBufferDesc, sizeof(D3D11_BUFFER_DESC));
            normalBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            normalBufferDesc.ByteWidth = sizeof(RVector3D) * mNormals.size();
            normalBufferDesc.CPUAccessFlags = 0;
            normalBufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
            resourceData.pSysMem = mNormals.data();
            HRESULT hr = GraphicsMain::GetDevice()->CreateBuffer(&normalBufferDesc, &resourceData, mNormalBuffer.GetAddressOf());

            if (FAILED(hr))
            {
                throw std::invalid_argument("Faile to create normal buffer");
            }
        }

        void RMesh::SetMaterialIndex(uint16_t index)
        {
            mMaterialIndex = index;
        }
    }
}