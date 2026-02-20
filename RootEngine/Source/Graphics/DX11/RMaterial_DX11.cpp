
#include "Graphics/DX11/RMaterial_DX11.h"
#include "Graphics/DX11/MaterialManager_DX11.h"
#include <stdexcept>

namespace Faia
{
    namespace Root
    {
        void RMaterial_DX11::SetShader(const char* shaderName)
        {
            _shader = MaterialManager_DX11::GetInstance()->LoadShader(shaderName);
        }

        void RMaterial_DX11::SetTexture(const char* texturePath, int32_t channel)
        {
            auto texture = MaterialManager_DX11::GetInstance()->LoadTexture2D(texturePath);
            _textures[channel] = texture;
        }

        Texture2D_DX11* RMaterial_DX11::GetTexture(int32_t channel)
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