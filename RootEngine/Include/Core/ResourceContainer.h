#pragma once
#include <iostream>
#include <memory>
#include <functional>

namespace Faia
{
    namespace Root
    {
        using OnDelete = std::function<void(uint32_t)>;

        class ResourceContainer
        {
            template<typename T>
            friend ResourceContainer CreateResourceContainer(OnDelete onDelete, uint32_t id);
        public:
            ResourceContainer();

            template<typename T>
            std::shared_ptr<T> GetShared()
            {
                mCountRef++;
                return std::shared_ptr<T>(GetRaw<T>(), [this](void* ptr)
                    {
                        OnDeleteShared(ptr);
                    });
            }

            template<typename T>
            T* GetRaw()
            {
                return static_cast<T*>(mRawPtr);
            }


        private:
            ResourceContainer(void* ptr, uint32_t id);
            void OnDeleteShared(void* ptr);
            OnDelete mOnDelete;
            void* mRawPtr;
            uint32_t mId;
            uint32_t mCountRef;

        };

        template<typename T>
        ResourceContainer CreateResourceContainer(OnDelete onDelete, uint32_t id)
        {
            ResourceContainer resourceContainer(new T(), id);
            resourceContainer.mOnDelete = onDelete;
            return resourceContainer;
        }
    }
}