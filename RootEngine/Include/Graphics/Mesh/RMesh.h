#pragma once
#include "Data/RVector3D.h"
#include "Core/RComponent.h"

#include "d3d11.h"

#include <wrl/client.h>
#include <vector>


namespace Faia
{
    namespace Root
    {   
        class RMesh
        {
        public:
            virtual ~RMesh() = default;

            void SetVertices(std::vector<RVector3D> verctices);
            void SetIndices(std::vector<uint16_t> indices);
            void SetUV(std::vector<RVector2D> uv);
            void SetNormals(std::vector<RVector3D> normals);
            void SetMaterialIndex(uint16_t index);

            ID3D11Buffer* GetVertexBuffer() { return mVertexBuffer.Get(); }
            ID3D11Buffer* GetIndexBuffer() { return mIndexBuffer.Get(); }
            ID3D11Buffer* GetUvBuffer() { return mUvBuffer.Get(); }
            ID3D11Buffer* GetNormalBuffer() { return mNormalBuffer.Get(); }

            std::vector<RVector3D> GetVertices() { return mVertices; }
            std::vector<RVector3D> GetNormals() { return mNormals; }
            std::vector<uint16_t> GetIndices() { return mIndices; }
            size_t GetIndicesSize() { return mIndices.size(); }
            uint16_t GetMaterialIndex() { return mMaterialIndex; }

        protected:
            Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
            Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
            Microsoft::WRL::ComPtr<ID3D11Buffer> mUvBuffer = nullptr;
            Microsoft::WRL::ComPtr<ID3D11Buffer> mNormalBuffer = nullptr;

            std::vector<uint16_t> mIndices;
            std::vector<RVector3D> mVertices;
            std::vector<RVector2D> mUv;
            std::vector<RVector3D> mNormals;
            uint16_t mMaterialIndex;

        };
    }
}