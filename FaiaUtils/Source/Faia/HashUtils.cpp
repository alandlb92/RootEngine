#include "HashUtils.h"
#include "Debug.h"
namespace Faia
{
    namespace HashUtils
    {
        std::map<uint32_t, std::string> g_hashToStringMap;

        uint32_t CharToHashFnv1a(const char* str)
        {
            std::string name(str);
            std::string msg(str);

            uint32_t hash = 2166136261u;
            while (*str) {
                hash ^= static_cast<uint32_t>(*str++);
                hash *= 16777619u;
            }

            g_hashToStringMap[hash] = name;
            msg.append(": ");
            msg.append(std::to_string(hash));
            Debug::Log(msg.c_str());
            return hash;
        }
    }
}
