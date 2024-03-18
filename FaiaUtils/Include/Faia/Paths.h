#pragma once
#include <string>
#include "windows.h"
#include <stdexcept>
//TODO: MAKE CROSS PLATFORM IF NECESSARY

namespace Faia
{
    namespace Paths
    {
        std::wstring GetApplicationFolderPath()
        {
            WCHAR path[MAX_PATH];
            GetModuleFileName(NULL, path, MAX_PATH);
            std::wstring fullPath(path);

     
            size_t lastSlashIndex = fullPath.find_last_of(L"\\");
            if (lastSlashIndex != std::wstring::npos) {     
                return fullPath.substr(0, lastSlashIndex + 1);
            }

            throw std::invalid_argument("Error when try to get application folder path");
            return L"";
        }

    }
}