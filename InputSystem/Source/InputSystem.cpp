#include "InputSystem.h"
#include <stdexcept>

InputSystem* InputSystem::_instance = nullptr;

InputSystem::InputSystem()
{
    _instance = this;
}

InputSystem::~InputSystem()
{
    _instance = nullptr;
}


void InputSystem::Update(float deltaTime)
{
    if (_events.size() > 0)
    {
        for (int i = 0;i < _events.size(); i++) 
        {
            auto mapOfEventType = _eventFunctions.find(_events[i].type);
            if (mapOfEventType != _eventFunctions.end())
            {
                auto listOfEvents = mapOfEventType->second.find(_events[i].key);
                if (listOfEvents != mapOfEventType->second.end())
                {
                    for (auto& evt : listOfEvents->second)
                    {
                        if (evt)
                        {
                            evt(deltaTime);
                        }
                    }
                }
            }
        }

        _events.clear();
    }
}

void InputSystem::RegisterActionEvent(InputEventType eventType, KeyCode keyCode, ActionEvent eventFunction)
{
    if (eventType == InputEventType::NONE || keyCode == KeyCode::INVALID)
        throw std::runtime_error("You can not add an InputEventType NONE or keyCode INVALID!");

    _eventFunctions[eventType][keyCode].push_back(eventFunction);
}

void InputSystem::SendOSEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    EventKeyCode eventKeyCode = InterpretOsEvent(message, wParam);

    if(eventKeyCode.type != InputEventType::NONE && eventKeyCode.key != KeyCode::INVALID)
        _events.push_back(eventKeyCode);

    if (message == WM_DESTROY)
    {
        PostQuitMessage(EXIT_SUCCESS);
    }
}


InputSystem::EventKeyCode InputSystem::InterpretOsEvent(UINT message, WPARAM wParam)
{
    KeyCode currentKeyCode = static_cast<KeyCode>(wParam);
    if (currentKeyCode != KeyCode::A)
        return EventKeyCode();

    EventKeyCode eventKeyCode;
    eventKeyCode.key = currentKeyCode;

    if (message == WM_KEYDOWN && keyStates[currentKeyCode] != KeyState::HOLD)
    {
        eventKeyCode.type = InputEventType::KEY_PRESSED;
        keyStates[currentKeyCode] = KeyState::HOLD;
    }
    else if (message == WM_KEYUP)
    {
        eventKeyCode.type = InputEventType::KEY_RELEASED;
        keyStates[currentKeyCode] = KeyState::IDLE;
    }
    else
    {
        eventKeyCode.type = InputEventType::NONE;
    }

    return eventKeyCode;
}
