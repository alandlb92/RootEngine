#include "Components/MeshComponent.h"

void MeshComponent::AddMeshs(std::vector<Mesh> meshs)
{
    for (const auto& mesh : meshs)
    {
        std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(mesh);
        _meshs.push_back(meshPtr);
    }
}

std::vector<std::shared_ptr<Mesh>> MeshComponent::GetMeshs()
{
    return _meshs;
}
