#include "Core/BObject.h"
#include "Faia/Debug.h"

namespace Faia
{
    namespace Root
    {
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
            if ((typeid(*component) == typeid(RMeshComponent) || typeid(*component) == typeid(RSkeletalMeshComponent)) && _meshComponent == nullptr)
            {
                _meshComponent = dynamic_cast<RMeshComponent*>(component);
            }
            else if (typeid(*component) == typeid(RMeshComponent))
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


            component->mOwner = this;
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

        void BObject::RegisterOnNotifyPositionChange(NotifyTrasnformChange evt)
        {
            _onNotifyPositionChange.push_back(evt);
        }

        void BObject::SetPosition(Vector3D newPosition)
        {
            _position = newPosition;
            for (auto& evt : _onNotifyPositionChange)
            {
                evt(_position);
            }
        }

        void BObject::SetRotation(Vector3D newRotation)
        {
            _rotation = newRotation;
            for (auto& evt : _onNotifyRotationChange)
            {
                evt(_rotation);
            }
        }

        void BObject::SetScale(Vector3D newScale)
        {
            _scale = newScale;
            for (auto& evt : _onNotifyScaleChange)
            {
                evt(_scale);
            }
        }

        void BObject::Update(float deltaTime)
        {
            for (auto& comp : _components)
            {
                comp->Update(deltaTime);
            }
        }

        void BObject::Init()
        {
            for (auto& comp : _components)
            {
                comp->Init();
            }
        }
    }
}