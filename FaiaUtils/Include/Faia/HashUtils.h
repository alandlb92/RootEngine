#pragma once
#include <ostream>
namespace Faia
{
    namespace HashUtils
    {
        uint32_t CharToHashFnv1a(const char* str);
    };
}