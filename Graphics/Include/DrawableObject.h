#pragma once
#include "Mesh.h"
#include "Shader.h"

class DrawableObject
{
public:
    DrawableObject();
    DrawableObject(Mesh mesh, Shader shader);

    Mesh GetMesh() { return _mesh; }
    Shader GetShader() { return _shader; }

private:
    Mesh _mesh;
    Shader _shader;

};

