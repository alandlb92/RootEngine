#pragma once
#include "Data/RMeshData.h"
#include "Core/ResourceContainer.h"
#include "Faia/HashUtils.h"
#include "Faia/Debug.h"

#include <filesystem>
#include <unordered_map>

namespace Faia 
{
    namespace Root 
    {
		class ResourcesManager
		{
        public:
            template<typename T>
            std::shared_ptr<T> Load(const char* relativePath)
            {
                uint32_t hash = HashUtils::CharToHashFnv1a(relativePath);

                if (mHashToResource.find(hash) == mHashToResource.end())
                {
                    std::filesystem::path fullPath = std::filesystem::current_path();
                    fullPath += pathToContent;
                    fullPath += relativePath;

                    if (!std::filesystem::exists(fullPath)) {
                        std::string msg("The file: ");
                        msg.append(fullPath.string());
                        msg.append(" Can't be found");
                        Debug::PopError(msg.c_str());
                    }
                    else {
                        std::string msg("get file from: ");
                        msg.append(fullPath.string());
                        Debug::Log(msg.c_str());
                    }

                    ResourceContainer rc = CreateResourceContainer<T>(std::bind(&ResourcesManager::DeleteResourceInMap, this, std::placeholders::_1), hash);
                    std::string s = fullPath.string();
                    rc.GetRaw<T>()->ReadFromPath(s.c_str());
                    mHashToResource[hash] = rc;
                }

                return mHashToResource[hash].GetShared<T>();
            }

        private:
            const char* pathToContent = "\\..\\Content\\";
            unordered_map<uint32_t, ResourceContainer> mHashToResource;
            void DeleteResourceInMap(uint32_t hash);

		};

        ResourcesManager* GetResourcesManager();
    }
}