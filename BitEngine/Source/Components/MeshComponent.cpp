#include "Components/MeshComponent.h"
#include "FaiaInputSystem.h"

void MeshComponent::AddMeshs(std::vector<std::shared_ptr<Mesh>> meshs)
{
    for (const auto& mesh : meshs)
    {
        _meshs.push_back(mesh);
    }
}

std::vector<std::shared_ptr<Mesh>> MeshComponent::GetMeshs()
{
    return _meshs;
}