#pragma once
#include "Mesh.h"
#include "Shader.h"

class DrawableObject
{
public:
    DrawableObject();
    DrawableObject(std::vector<Mesh> mesh, Shader shader);

    std::vector<Mesh> GetMeshs() { return _meshs; }
    Shader GetShader() { return _shader; }

private:
    std::vector<Mesh> _meshs;
    Shader _shader;

};

