#include "pch.h"
#include "MaterialManager.h"


std::unique_ptr<MaterialManager> MaterialManager::_instance = nullptr;

std::shared_ptr<Shader> MaterialManager::LoadShader(const char* shaderName)
{
    if (_shadersMap.find(shaderName) != _shadersMap.end())
    {
        return _shadersMap[shaderName];
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->Load(shaderName);  
    _shadersMap[shaderName] = shader;

    return shader;
}

std::shared_ptr<Texture2D> MaterialManager::LoadTexture2D(const char* texturePath)
{

    if (_texturesMap.find(texturePath) != _texturesMap.end())
    {
        return _texturesMap[texturePath];
    }

    std::shared_ptr<Texture2D> texture2D = std::make_shared<Texture2D>();
    texture2D->Load(texturePath);
    _texturesMap[texturePath] = texture2D;

    return texture2D;
}
