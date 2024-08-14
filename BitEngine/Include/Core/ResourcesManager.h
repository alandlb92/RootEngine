#pragma once
#include "Data/RMeshData.h"
#include "Core/ResourceContainer.h"
#include "Faia/HashUtils.h"

#include <filesystem>
#include <unordered_map>

namespace Faia 
{
    namespace Root 
    {
        class ResourcesManager;

        ResourcesManager* GetResourcesManager();

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
                    ResourceContainer rc = ResourceContainer::Create<T>(std::bind(&ResourcesManager::DeleteResourceInMap, this, std::placeholders::_1), hash);
                    std::string s = fullPath.string();
                    rc.GetRaw<T>()->ReadFromPath(s.c_str());
                    mHashToResource[hash] = rc;
                }

                return mHashToResource[hash].GetShared<T>();
            }

        private:
            //todo: this path just work in vs, we need to create config files to get the paths
#ifdef _DEBUG
            const char* pathToContent = "\\..\\X64\\Debug\\Content\\";
#elif _NDEBUG
            const char* pathToContent = "\\Content\\";
#endif
            unordered_map<uint32_t, ResourceContainer> mHashToResource;
            void DeleteResourceInMap(uint32_t hash);

		};
    }
}