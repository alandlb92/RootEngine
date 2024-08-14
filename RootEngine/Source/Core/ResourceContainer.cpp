#include "Core/ResourceContainer.h"

namespace Faia
{
    namespace Root
    {        
        ResourceContainer::ResourceContainer()
        {
            mRawPtr = nullptr;
            mCountRef = 0;
        }

        ResourceContainer::ResourceContainer(void* ptr, uint32_t id)
        {
            mId = id;
            mRawPtr = ptr;
            mCountRef = 0;
        }

        void ResourceContainer::OnDeleteShared(void* ptr)
        {
            if (--mCountRef == 0)
            {
                delete mRawPtr;
                mRawPtr = nullptr;
                mOnDelete(mId);
            }
        }
    }
}