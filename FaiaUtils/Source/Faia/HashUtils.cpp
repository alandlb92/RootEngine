#include "HashUtils.h"

//Todo: put this utils in test code

namespace Faia
{
    //Todo: a way to return hash to char
    uint32_t HashUtils::CharToHashFnv1a(const char* str)
    {
        uint32_t hash = 2166136261u;
        while (*str) {
            hash ^= static_cast<uint32_t>(*str++);
            hash *= 16777619u;
        }
        return hash;
    }
}
