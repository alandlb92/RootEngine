#include "Debug.h"
#include <string>
#include <cstdio>

namespace Faia
{
    void Debug::Log(const char* msg)
    {
        std::string s(msg);
        s.push_back('\n');
        OutputDebugStringA(s.c_str());
    }

    void Debug::PopError(const char* msg)
    {
        std::string s(msg);
        s.push_back('\n');
        MessageBoxA(NULL, s.c_str(), "Error", MB_ICONERROR | MB_OK);
        DebugBreak();
    }
}