#pragma once
#include <ostream>
namespace Faia
{
    class HashUtils
    {
    public:
        static uint32_t CharToHashFnv1a(const char* str);
    };
}