#pragma once
#include <windows.h>

enum class KeyCode
{
    A,
	S,
	D,
	W
};

class InputSystem
{
	friend class WindowsApplication;
	
public:
	InputSystem() = default;
	~InputSystem() = default;
	
private:
	void SendOSEvent(UINT message, WPARAM wParam, LPARAM lParam);
	CHAR GetChar(WPARAM wparam);
};