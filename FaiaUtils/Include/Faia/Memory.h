#pragma once
namespace Faia
{
    namespace Memory
    {
        // Safely release a COM object.
        template<typename T>
        inline void SafeRelease(T& ptr)
        {
            if (ptr != NULL)
            {
                ptr->Release();
                ptr = NULL;
            }
        }
    }
}