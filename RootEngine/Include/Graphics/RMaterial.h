#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include <memory>
#include <map>

namespace Faia
{
    namespace Root
    {
        class RMaterial
        {
        public:
            void SetShader(const char* shaderName);
            void SetTexture(const char* texturePath, int32_t channel);
            Shader* GetShader() { return _shader.get(); }
            Texture2D* GetTexture(int32_t channel);

        private:
            std::shared_ptr<Shader> _shader;
            std::map<uint32_t, std::shared_ptr<Texture2D>> _textures;
        };
    }
}