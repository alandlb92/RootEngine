#pragma once
#include "Shader_DX11.h"
#include "Texture2D_DX11.h"
#include <map>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class MaterialManager_DX11
        {
        public:
            static MaterialManager_DX11* GetInstance()
            {
                if (_instance == nullptr)
                {
                    _instance = std::make_unique<MaterialManager_DX11>();
                }

                return _instance.get();
            };
            std::shared_ptr<Shader_DX11> LoadShader(const char* shaderName);
            std::shared_ptr<Texture2D_DX11> LoadTexture2D(const char* texturePath);

        private:
            static std::unique_ptr<MaterialManager_DX11> _instance;

            std::map<const char*, std::shared_ptr<Shader_DX11>> _shadersMap;
            std::map<const char*, std::shared_ptr<Texture2D_DX11>> _texturesMap;
        };
    }
}