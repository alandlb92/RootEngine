#pragma once
#include <ostream>
#include <map>
#include <string>

namespace Faia
{
    namespace HashUtils
    {
        extern std::map<uint32_t, std::string> g_hashToStringMap;

        uint32_t CharToHashFnv1a(const char* str);
    }
}