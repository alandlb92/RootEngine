#pragma once
#include "Windows.h"
namespace Faia
{
    namespace Debug
    {
         void Log(const char* msg);
         void PopError(const char* msg);
    };
}