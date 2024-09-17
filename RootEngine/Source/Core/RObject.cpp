#include "Core/RObject.h"
#include "Faia/Debug.h"

namespace Faia
{
    namespace Root
    {
        using namespace Faia::Debug;

        RObject::RObject()
        {
            _rotation = RVector3D(0.0f);
            _position = RVector3D(0.0f);
            _scale = RVector3D(1.0f);
            _components = {};
        }

        void RObject::AddComponent(RComponent* component)
        {
            if ((typeid(*component) == typeid(RMeshComponent) || typeid(*component) == typeid(RSkeletalMeshComponent)) && _meshComponent == nullptr)
            {
                _meshComponent = dynamic_cast<RMeshComponent*>(component);
            }
            else if (typeid(*component) == typeid(RMeshComponent))
            {
                PopError("You cant have more than 1 MeshComponent in the sabe Object");
            }

            if (typeid(*component) == typeid(RMaterialComponent) && _materialComponent == nullptr)
            {
                _materialComponent = dynamic_cast<RMaterialComponent*>(component);
            }
            else if (typeid(*component) == typeid(RMaterialComponent))
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

        void RObject::RegisterOnNotifyComponentAddedEvent(NotifyAddComponentEvent evt)
        {
            _onNotifyComponentAddedList.push_back(evt);
        }

        void RObject::RegisterOnNotifyPositionChange(NotifyTrasnformChange evt)
        {
            _onNotifyPositionChange.push_back(evt);
        }

        void RObject::RegisterOnNotifyRotationChange(NotifyTrasnformChange evt)
        {
            _onNotifyRotationChange.push_back(evt);
        }

        void RObject::SetPosition(RVector3D newPosition)
        {
            _position = newPosition;
            for (auto& evt : _onNotifyPositionChange)
            {
                evt(_position);
            }
        }

        void RObject::SetRotation(RVector3D newRotation)
        {
            _rotation = newRotation;
            for (auto& evt : _onNotifyRotationChange)
            {
                evt(_rotation);
            }
        }

        void RObject::SetScale(RVector3D newScale)
        {
            _scale = newScale;
            for (auto& evt : _onNotifyScaleChange)
            {
                evt(_scale);
            }
        }

        void RObject::Update(float deltaTime)
        {
            for (auto& comp : _components)
            {
                comp->Update(deltaTime);
            }
        }

        void RObject::Init()
        {
            for (auto& comp : _components)
            {
                comp->Init();
            }
        }
    }
}