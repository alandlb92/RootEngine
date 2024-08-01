#pragma once
#include "wrl/client.h"
#include "d3d11.h"

namespace Faia
{
    namespace Root
    {
        class Texture2D
        {
        public:
            ID3D11ShaderResourceView* GetTexture() { return _textureSRV.Get(); }
            ID3D11SamplerState* GetSamplerState() { return _samplerState.Get(); }
            void Load(const char* textureRelativePath);

        private:
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;
            Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerState;

        };
    }
}