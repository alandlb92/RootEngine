#include "Core/ResourcesManager.h"


namespace Faia
{
    namespace Root
    {
        ResourcesManager* gResourceManager;
        ResourcesManager* GetResourcesManager()
        {
            if (gResourceManager == nullptr)
            {
                gResourceManager = new ResourcesManager();
            }

            return gResourceManager;
        }

        void ResourcesManager::DeleteResourceInMap(uint32_t hash)
        {
            mHashToResource.erase(hash);
        }
    }
}