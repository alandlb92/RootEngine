#include "HashUtils.h"
#include <unordered_map>

namespace Faia
{
    std::unordered_map<uint32_t, std::string> g_hashToStringMap;

    uint32_t HashUtils::CharToHashFnv1a(const char* str)
    {
        uint32_t hash = 2166136261u;
        while (*str) {
            hash ^= static_cast<uint32_t>(*str++);
            hash *= 16777619u;
        }

        g_hashToStringMap[hash] = std::string(str);

        return hash;
    }
}
