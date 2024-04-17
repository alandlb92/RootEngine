#pragma once
#include "Core/BComponent.h"
#include "Graphics/Material.h"
#include <vector>
#include <memory>

class MaterialComponent : public BComponent
{
public:
    void AddMaterial(Material material);
    Material* GetMaterialOfIndex(int index);

private:
    std::vector<std::shared_ptr<Material>> _materials;

};