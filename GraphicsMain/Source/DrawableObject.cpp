#include "pch.h"
#include "DrawableObject.h"

DrawableObject::DrawableObject()
{
    DrawableObject(Mesh::MakePrimitiveCube(), Shader());
}

DrawableObject::DrawableObject(Mesh mesh, Shader shader)
{
    _mesh = mesh;
    _shader = shader;
}
