#include "Core/BObject.h"
#include "Faia/Debug.h"

using namespace Faia::Debug;

BObject::BObject()
{
    _rotation = Vector3D(0.0f);
    _position = Vector3D(0.0f);
    _scale = Vector3D(1.0f);
    _components = {};
}

void BObject::AddComponent(BComponent* component)
{
    if (typeid(*component) == typeid(MeshComponent) && _meshComponent == nullptr)
    {
        _meshComponent = dynamic_cast<MeshComponent*>(component);
    }
    else if(typeid(*component) == typeid(MeshComponent))
    {
        PopError("You cant have more than 1 MeshComponent in the sabe Object");
    }

    if (typeid(*component) == typeid(MaterialComponent) && _materialComponent == nullptr)
    {
        _materialComponent = dynamic_cast<MaterialComponent*>(component);
    }
    else if (typeid(*component) == typeid(MaterialComponent))
    {
        PopError("You cant have more than 1 MaterialComponent in the sabe Object");
    }


    _components.push_back(component);
    for (auto& onNotifyComponentAdded : _onNotifyComponentAddedList)
    {
        onNotifyComponentAdded(this, component);
    }
}

void BObject::RegisterOnNotifyComponentAddedEvent(NotifyAddComponentEvent evt)
{
    _onNotifyComponentAddedList.push_back(evt);
}

void BObject::Update(float deltaTime)
{
}

void BObject::Init()
{
}
