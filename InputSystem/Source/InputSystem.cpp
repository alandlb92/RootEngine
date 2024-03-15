#include "pch.h"
#include "InputSystem.h"

void InputSystem::SendOSEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        break;
    // key down    
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_CONTROL:            
            OutputDebugStringA("Key DOWN \n");
            break;
        case VK_SHIFT:
            OutputDebugStringA("Shift DOWN \n");
            break;
        }

        break;
    // key down    
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_CONTROL:
            OutputDebugStringA("Key UP");
            break;
        case VK_SHIFT:
            OutputDebugStringA("Shift UP");
            break;
        }

        break;
    // key down    
    case WM_CHAR:
        if (GetChar(wParam))
        {
            MessageBox(NULL, TEXT("Character Pressed"), TEXT("char"), MB_OK);
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
    }
}

CHAR InputSystem::GetChar(WPARAM wparam)  
{  
    CHAR a = NULL;  
    for (char ch = 'a'; ch <= 'z'; ch++)  
    {  
        if (wparam == ch)  
        {  
            a = ch;  
        }  
    }  
    for (char ch = 'A'; ch <= 'Z'; ch++)  
    {  
        if (wparam == ch)  
        {  
            a = ch;  
        }  
    }  
    return a;  
} 