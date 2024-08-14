
#include "Graphics/RMaterial.h"
#include "Graphics/MaterialManager.h"
#include <stdexcept>

namespace Faia
{
    namespace Root
    {
        void RMaterial::SetShader(const char* shaderName)
        {
            _shader = MaterialManager::GetInstance()->LoadShader(shaderName);
        }

        void RMaterial::SetTexture(const char* texturePath, int32_t channel)
        {
            auto texture = MaterialManager::GetInstance()->LoadTexture2D(texturePath);
            _textures[channel] = texture;
        }

        Texture2D* RMaterial::GetTexture(int32_t channel)
        {
            if (_textures.size() > 0)
            {
                auto it = _textures.find(channel);
                if (it == _textures.end())
                {
                    throw std::invalid_argument("Invalid channel passed to get texture from material");
                }

                return _textures[channel].get();
            }
            else
            {
                return NULL;
            }
        }
    }
}