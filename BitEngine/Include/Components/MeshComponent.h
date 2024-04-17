#pragma once
#include "Core/BComponent.h"
#include "Graphics/Mesh.h"
#include <vector>
#include <memory>

class MeshComponent : public BComponent
{
public:
    void AddMeshs(std::vector<Mesh> mesh);
    std::vector<std::shared_ptr<Mesh>> GetMeshs();

private:
    std::vector<std::shared_ptr<Mesh>> _meshs;

};