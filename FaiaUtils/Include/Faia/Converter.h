#pragma once
#include "Windows.h"
namespace Faia
{
    class Converter
    {
    public:
        static LPCWSTR CharToLPCWSTR(const char* cStr); 
        const char* LPCWSTRToChar(LPCWSTR wideStr);

    };
}