#pragma once
#include "Data/RVector3D.h"
#include "Core/RComponent.h"

#include "Components/RMaterialComponent.h"
#include "Components/RSkeletalMeshComponent.h"

#include <list>
#include <functional>

namespace Faia
{
    namespace Root
    {
        class RObject;

        using NotifyAddComponentEvent = std::function<void(RObject* obj, RComponent* newComponent)>;
        using NotifyTrasnformChange = std::function<void(RVector3D)>;

        class RObject
        {
            friend class Scene;
        public:
            RObject();
            virtual void AddComponent(RComponent* component);
            void RegisterOnNotifyComponentAddedEvent(NotifyAddComponentEvent evt);
            void RegisterOnNotifyPositionChange(NotifyTrasnformChange evt);

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

            RMeshComponent* GetMeshComponent() const { return _meshComponent; }
            RMaterialComponent* GetMaterialComponent() const { return _materialComponent; }

            void SetPosition(RVector3D newPosition);
            void SetRotation(RVector3D newRotation);
            void SetScale(RVector3D newScale);

            RVector3D GetPosition() { return _position; }
            RVector3D GetRotation() { return _rotation; }
            RVector3D GetScale() { return _scale; }


        protected:
            virtual void Init();
            virtual void Update(float deltaTime);

            std::list<RComponent*> _components;
            RVector3D _rotation;
            RVector3D _position;
            RVector3D _scale;

        private:
            std::list<NotifyAddComponentEvent> _onNotifyComponentAddedList;
            std::list<NotifyTrasnformChange> _onNotifyPositionChange;
            std::list<NotifyTrasnformChange> _onNotifyRotationChange;
            std::list<NotifyTrasnformChange> _onNotifyScaleChange;
            RMeshComponent* _meshComponent;
            RMaterialComponent* _materialComponent;
        };         
    }
}