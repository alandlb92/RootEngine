#include "Graphics/AnimationManager.h"
#include "Data/BitMeshData.h"
#include "Faia/HashUtils.h"

std::unique_ptr<AnimationManager> AnimationManager::sInstance = nullptr;

//Todo: we need to make sure about this uses of smart pointers
RAnimationData* AnimationManager::LoadAnimationFromPath(const char* path)
{
    uint32_t animHash = Faia::HashUtils::CharToHashFnv1a(path);

    auto it = mAnimationMap.find(animHash);
    if (it != mAnimationMap.end())
    {
        return it->second.get();
    }
    else
    {
        std::shared_ptr<RAnimationData> animationData = std::make_shared<RAnimationData>();
        animationData->ReadFromPath(path);
        mAnimationMap[animHash] = animationData;
        return animationData.get();
    }
}
