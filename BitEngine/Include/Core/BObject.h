#pragma once
#include "Data/Vector3D.h"
#include "Core/BComponent.h"

#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"

#include <list>
#include <functional>

namespace Faia
{
    namespace Root
    {
        class BObject;

        using NotifyAddComponentEvent = std::function<void(BObject* obj, BComponent* newComponent)>;
        using NotifyTrasnformChange = std::function<void(Vector3D)>;

        class BObject
        {
            friend class Scene;
        public:
            BObject();
            virtual void AddComponent(BComponent* component);
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

            MeshComponent* GetMeshComponent() const { return _meshComponent; }
            MaterialComponent* GetMaterialComponent() const { return _materialComponent; }

            void SetPosition(Vector3D newPosition);
            void SetRotation(Vector3D newRotation);
            void SetScale(Vector3D newScale);

            Vector3D GetPosition() { return _position; }
            Vector3D GetRotation() { return _rotation; }
            Vector3D GetScale() { return _scale; }


        protected:
            virtual void Init();
            virtual void Update(float deltaTime);

            std::list<BComponent*> _components;
            Vector3D _rotation;
            Vector3D _position;
            Vector3D _scale;

        private:
            std::list<NotifyAddComponentEvent> _onNotifyComponentAddedList;
            std::list<NotifyTrasnformChange> _onNotifyPositionChange;
            std::list<NotifyTrasnformChange> _onNotifyRotationChange;
            std::list<NotifyTrasnformChange> _onNotifyScaleChange;
            MeshComponent* _meshComponent;
            MaterialComponent* _materialComponent;
        };         
    }
}