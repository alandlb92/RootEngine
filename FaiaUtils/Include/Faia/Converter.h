#pragma once
#include "Windows.h"
namespace Faia
{
    namespace Converter
    {
        LPCWSTR CharToLPCWSTR(const char* cStr); 
        const char* LPCWSTRToChar(LPCWSTR wideStr);
    };
}