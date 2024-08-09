#include "Graphics/DrawableObject.h"

namespace Faia
{
    namespace Root
    {
        DrawableObject::DrawableObject(std::vector<RMesh> meshs, std::vector<Material> materials)
        {
            _meshs = meshs;
            _materials = materials;
        }
    }
}