#pragma once
#include "Shader_DX11.h"
#include "Texture2D_DX11.h"
#include <memory>
#include <map>

namespace Faia
{
    namespace Root
    {
        class RMaterial_DX11
        {
        public:
            void SetShader(const char* shaderName);
            void SetTexture(const char* texturePath, int32_t channel);
            Shader_DX11* GetShader() { return _shader.get(); }
            Texture2D_DX11* GetTexture(int32_t channel);

        private:
            std::shared_ptr<Shader_DX11> _shader;
            std::map<uint32_t, std::shared_ptr<Texture2D_DX11>> _textures;
        };
    }
}