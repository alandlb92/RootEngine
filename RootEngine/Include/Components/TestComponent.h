#pragma once
#include "Core/RComponent.h"
#include "FaiaInputSystem.h"
#include "Core/RObject.h"
#include "Core/Scene/SceneManager.h"
#include "RDirectionalLightComponent.h"
#include "Data/RColor.h"

namespace Faia
{
    namespace Root
    {
        class TestComponent : public RComponent
        {
            typedef RComponent Super;


        public:
            float objVelocity = 150.0f;

            virtual void Init() override
            {
                Super::Init();
                Faia::InputSystem::GetFaiaInputSystem()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_RIGHT, std::bind(&TestComponent::MoveXP, this, std::placeholders::_1));
                Faia::InputSystem::GetFaiaInputSystem()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_LEFT, std::bind(&TestComponent::MoveXM, this, std::placeholders::_1));

                Faia::InputSystem::GetFaiaInputSystem()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_UP, std::bind(&TestComponent::MoveUp, this, std::placeholders::_1));
                Faia::InputSystem::GetFaiaInputSystem()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::ARROW_DOWN, std::bind(&TestComponent::MoveDown, this, std::placeholders::_1));


                Faia::InputSystem::GetFaiaInputSystem()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::Q, std::bind(&TestComponent::MoveZM, this, std::placeholders::_1));
                Faia::InputSystem::GetFaiaInputSystem()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
                    Faia::InputSystem::KeyCode::E, std::bind(&TestComponent::MoveZB, this, std::placeholders::_1));

                GetSceneManager()->GetComponentOfType<RDirectionalLightComponent>()->SetColor(RColorRGB(0, 1, 0));
                GetSceneManager()->GetComponentOfType<RDirectionalLightComponent>()->SetStrength(1);

            }

        private:
            void DumpPosition()
            {
                Debug::Log(mOwner->GetRotation().ToString().c_str());
            }

            void MoveUp(float deltaTime)
            {
                if (mOwner)
                {
                    RVector3D curPos = mOwner->GetRotation();
                    curPos += RVector3D(-objVelocity * deltaTime, 0, 0);
                    mOwner->SetRotation(curPos);
                    DumpPosition();
                }
            }

            void MoveDown(float deltaTime)
            {
                if (mOwner)
                {
                    RVector3D curPos = mOwner->GetRotation();
                    curPos += RVector3D(objVelocity * deltaTime, 0, 0);
                    mOwner->SetRotation(curPos);
                    DumpPosition();
                }
            }

            void MoveXP(float deltaTime)
            {
                if (mOwner)
                {
                    RVector3D curPos = mOwner->GetRotation();
                    curPos += RVector3D(0, -objVelocity * deltaTime, 0);
                    mOwner->SetRotation(curPos);
                    DumpPosition();
                }
            }

            void MoveXM(float deltaTime)
            {
                if (mOwner)
                {
                    RVector3D curPos = mOwner->GetRotation();
                    curPos += RVector3D(0, objVelocity * deltaTime, 0);
                    mOwner->SetRotation(curPos);
                    DumpPosition();
                }
            }

            void MoveZM(float deltaTime)
            {
                if (mOwner)
                {
                    RVector3D curPos = mOwner->GetRotation();
                    curPos += RVector3D(0, 0, -objVelocity * deltaTime);
                    mOwner->SetRotation(curPos);
                    DumpPosition();
                }
            }

            void MoveZB(float deltaTime)
            {
                if (mOwner)
                {
                    RVector3D curPos = mOwner->GetRotation();
                    curPos += RVector3D(0, 0, objVelocity * deltaTime);
                    mOwner->SetRotation(curPos);
                    DumpPosition();
                }
            }

        };
    }
}