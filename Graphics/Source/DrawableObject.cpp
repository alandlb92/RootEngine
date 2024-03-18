#include "pch.h"
#include "DrawableObject.h"

DrawableObject::DrawableObject(std::vector<Mesh> meshs, std::vector<Material> materials)
{
    _meshs = meshs;
    _materials = materials;
}
