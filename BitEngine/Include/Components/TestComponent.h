#pragma once
#include "Core/BComponent.h"
#include "FaiaInputSystem.h"
#include "Core/BObject.h"

namespace Faia
{
    namespace Root
    {
        class TestComponent : public BComponent
        {
            typedef BComponent Super;


        public:
            float objVelocity = 150.0f;

            virtual void Init() override
            {
                Super::Init();
                Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_RIGHT, std::bind(&TestComponent::MoveRight, this, std::placeholders::_1));
                Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_LEFT, std::bind(&TestComponent::MoveLeft, this, std::placeholders::_1));

                Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_UP, std::bind(&TestComponent::MoveUp, this, std::placeholders::_1));
                Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_DOWN, std::bind(&TestComponent::MoveDown, this, std::placeholders::_1));


                Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::KEYPAD_2, std::bind(&TestComponent::MoveFoward, this, std::placeholders::_1));
                Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::KEYPAD_8, std::bind(&TestComponent::MoveBack, this, std::placeholders::_1));

            }

        private:
            void MoveUp(float deltaTime)
            {
                if (mOwner)
                {
                    Vector3D curPos = mOwner->GetPosition();
                    curPos += Vector3D(0, objVelocity * deltaTime, 0);
                    mOwner->SetPosition(curPos);
                }
            }

            void MoveDown(float deltaTime)
            {
                if (mOwner)
                {
                    Vector3D curPos = mOwner->GetPosition();
                    curPos += Vector3D(0, -objVelocity * deltaTime, 0);
                    mOwner->SetPosition(curPos);
                }
            }

            void MoveRight(float deltaTime)
            {
                if (mOwner)
                {
                    Vector3D curPos = mOwner->GetPosition();
                    curPos += Vector3D(-objVelocity * deltaTime, 0, 0);
                    mOwner->SetPosition(curPos);
                }
            }

            void MoveLeft(float deltaTime)
            {
                if (mOwner)
                {
                    Vector3D curPos = mOwner->GetPosition();
                    curPos += Vector3D(objVelocity * deltaTime, 0, 0);
                    mOwner->SetPosition(curPos);
                }
            }

            void MoveFoward(float deltaTime)
            {
                if (mOwner)
                {
                    Vector3D curPos = mOwner->GetPosition();
                    curPos += Vector3D(0, 0, objVelocity * deltaTime);
                    mOwner->SetPosition(curPos);
                }
            }

            void MoveBack(float deltaTime)
            {
                if (mOwner)
                {
                    Vector3D curPos = mOwner->GetPosition();
                    curPos += Vector3D(0, 0, -objVelocity * deltaTime);
                    mOwner->SetPosition(curPos);
                }
            }

        };
    }
}