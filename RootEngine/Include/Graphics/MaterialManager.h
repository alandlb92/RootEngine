#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include <map>
#include <memory>

namespace Faia
{
    namespace Root
    {
        class MaterialManager
        {
        public:
            static MaterialManager* GetInstance()
            {
                if (_instance == nullptr)
                {
                    _instance = std::make_unique<MaterialManager>();
                }

                return _instance.get();
            };
            std::shared_ptr<Shader> LoadShader(const char* shaderName);
            std::shared_ptr<Texture2D> LoadTexture2D(const char* texturePath);

        private:
            static std::unique_ptr<MaterialManager> _instance;

            std::map<const char*, std::shared_ptr<Shader>> _shadersMap;
            std::map<const char*, std::shared_ptr<Texture2D>> _texturesMap;
        };
    }
}