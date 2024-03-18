#include "pch.h"
#include "DrawableObject.h"

DrawableObject::DrawableObject()
{
    DrawableObject(std::vector<Mesh>{Mesh::MakePrimitiveCube()}, Shader());
}

DrawableObject::DrawableObject(std::vector<Mesh> meshs, Shader shader)
{
    _meshs = meshs;
    _shader = shader;
}
