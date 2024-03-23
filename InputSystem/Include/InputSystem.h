#pragma once
#include <windows.h>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <cassert>


enum class InputEventType 
{
	KEY_PRESSED,
	KEY_RELEASED, 
	KEY_HELD,
	KEY_IDLE,
	NONE
};

enum class KeyCode
{
	INVALID = 0,
    A = 0x41,
	S = 0x53,
	D = 0x44,
	W = 0x57
};


class InputSystem
{
	using ActionEvent = std::function<void(float deltaTime)>;
	friend class WindowsApplication;
	friend class BitEngine;
	
public:
	InputSystem();
	~InputSystem();
	static InputSystem* GetInstance()
	{
		assert(_instance);
		return _instance;
	}


	void RegisterActionEvent(InputEventType  eventType, KeyCode keyCode, ActionEvent actionEvent);

private:
	struct EventKeyCode
	{
		InputEventType type;
		KeyCode key;

		EventKeyCode()
		{
			type = InputEventType::NONE;
			key = KeyCode::INVALID;
		}
	};
	std::vector<EventKeyCode> _events;


	std::map<InputEventType, std::map<KeyCode,std::vector<ActionEvent>>> _eventFunctions;
	std::set<KeyCode> _heldKeys;

	void Update(float deltaTime);
	void SendOSEvent(UINT message, WPARAM wParam, LPARAM lParam);
	
	void FindEvtAndCall(InputEventType evtType, KeyCode keyCode, float deltaTime);

	EventKeyCode InterpretOsEvent(UINT message, WPARAM wParam);
	

	static InputSystem* _instance;
};