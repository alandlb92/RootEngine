#pragma once
#include "Core/BComponent.h"
#include "FaiaInputSystem.h"
#include "Core/BObject.h"

class TestComponent : public BComponent
{
    typedef BComponent Super;

public:
    virtual void Init() override
    {
        Super::Init();
        Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
            Faia::InputSystem::KeyCode::Q, std::bind(&TestComponent::MoveRight, this, std::placeholders::_1));
        Faia::InputSystem::FaiaInputSystem::GetInstance()->RegisterActionEvent(Faia::InputSystem::InputEventType::KEY_HELD,
            Faia::InputSystem::KeyCode::E, std::bind(&TestComponent::MoveLeft, this, std::placeholders::_1));

    }

private:
    void MoveRight(float deltaTime)
    {
        if (_owner)
        {
            Vector3D curPos = _owner->GetPosition();
            curPos += Vector3D(50 * deltaTime, 0, 0);
            _owner->SetPosition(curPos);
        }
    }

    void MoveLeft(float deltaTime)
    {
        if (_owner)
        {
            Vector3D curPos = _owner->GetPosition();
            curPos += Vector3D(-50 * deltaTime, 0, 0);
            _owner->SetPosition(curPos);
        }
    }

};