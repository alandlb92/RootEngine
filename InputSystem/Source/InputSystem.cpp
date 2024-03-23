#include "InputSystem.h"
#include <stdexcept>
#include <sstream>

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
    //Pressed and Released events
    if (_events.size() > 0)
    {
        for (int i = 0;i < _events.size(); i++) 
        {
            FindEvtAndCall(_events[i].type, _events[i].key, deltaTime);
        }

        _events.clear();
    }

    //Held events
    for (auto& heldKey : _heldKeys)
    {
            FindEvtAndCall(InputEventType::KEY_HELD, heldKey, deltaTime);
     
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
    EventKeyCode eventKeyCode;
    eventKeyCode.key = currentKeyCode;

    auto it = _heldKeys.find(currentKeyCode);
    if (message == WM_KEYDOWN && it == _heldKeys.end())
    {
        eventKeyCode.type = InputEventType::KEY_PRESSED;
        _heldKeys.insert(currentKeyCode);
    }
    else if (message == WM_KEYUP)
    {
        eventKeyCode.type = InputEventType::KEY_RELEASED;
        _heldKeys.erase(currentKeyCode);
    }
    else
    {
        eventKeyCode.type = InputEventType::NONE;
    }

    return eventKeyCode;
}

void InputSystem::FindEvtAndCall(InputEventType evtType, KeyCode keyCode, float deltaTime)
{
    auto mapOfEventType = _eventFunctions.find(evtType);
    if (mapOfEventType != _eventFunctions.end())
    {
        auto listOfEvents = mapOfEventType->second.find(keyCode);
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
