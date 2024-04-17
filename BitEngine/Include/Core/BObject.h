#pragma once
#include "Data/Vector3D.h"
#include "Core/BComponent.h"

#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"

#include <list>
#include <functional>

class BObject;

using NotifyAddComponentEvent = std::function<void(BObject* obj, BComponent* newComponent)>;

class BObject 
{

    friend class Scene;
public:
    BObject();
    virtual void AddComponent(BComponent* component);
    void RegisterOnNotifyComponentAddedEvent(NotifyAddComponentEvent evt);

    template<typename T>
    T* GetComponentOfType()
    {
        for (auto& component : _components)
        {
            if (typeid(*component) == typeid(T))
            {
                return dynamic_cast<T*>(component);
            }
        }

        return nullptr;
    }

    MeshComponent* GetMeshComponent() const { return _meshComponent; }
    MaterialComponent* GetMaterialComponent() const { return _materialComponent; }

protected:
    virtual void Init();
    virtual void Update(float deltaTime);

    std::list<BComponent*> _components;
    Vector3D _rotation;
    Vector3D _position;
    Vector3D _scale;

private:
    std::list<NotifyAddComponentEvent> _onNotifyComponentAddedList;
    MeshComponent* _meshComponent;
    MaterialComponent* _materialComponent;
};