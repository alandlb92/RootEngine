#include "Graphics/DX11/MaterialManager_DX11.h"


namespace Faia
{
    namespace Root
    {
        std::unique_ptr<MaterialManager_DX11> MaterialManager_DX11::_instance = nullptr;

        std::shared_ptr<Shader_DX11> MaterialManager_DX11::LoadShader(const char* shaderName)
        {
            if (_shadersMap.find(shaderName) != _shadersMap.end())
            {
                return _shadersMap[shaderName];
            }

            std::shared_ptr<Shader_DX11> shader = std::make_shared<Shader_DX11>();
            shader->Load(shaderName);
            _shadersMap[shaderName] = shader;

            return shader;
        }

        //Todo: implement resource manager
        std::shared_ptr<Texture2D_DX11> MaterialManager_DX11::LoadTexture2D(const char* texturePath)
        {

            if (_texturesMap.find(texturePath) != _texturesMap.end())
            {
                return _texturesMap[texturePath];
            }

            std::shared_ptr<Texture2D_DX11> texture2D = std::make_shared<Texture2D_DX11>();
            texture2D->Load(texturePath);
            _texturesMap[texturePath] = texture2D;

            return texture2D;
        }
    }
}