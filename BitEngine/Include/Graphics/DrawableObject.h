#pragma once
#include "Mesh/RMesh.h"
#include "Material.h"

namespace Faia
{
    namespace Root
    {
        //Todo: delete this class?
        class DrawableObject
        {
        public:
            DrawableObject(std::vector<RMesh> mesh, std::vector<Material> materials);

            std::vector<RMesh> GetMeshs() { return _meshs; }
            Material GetMaterial(unsigned int index)
            {
                if (index >= _materials.size())
                    return _materials[0]; //TODO: instead return material index 0, return a dummy material, like in unity when you have a pink color

                return _materials[index];
            }

        private:
            std::vector<RMesh> _meshs;
            std::vector<Material> _materials;

        };
    }
}