#include "Graphics/AnimationManager.h"
#include "Data/BitMeshData.h"
#include "Faia/HashUtils.h"

std::unique_ptr<AnimationManager> AnimationManager::sInstance = nullptr;

std::weak_ptr<RAnimationData> AnimationManager::LoadAnimationFromPath(const char* path)
{
    uint32_t animHash = Faia::HashUtils::CharToHashFnv1a(path);

    auto it = mAnimationMap.find(animHash);
    if (it != mAnimationMap.end())
    {
        return it->second;
    }
    else
    {
        std::shared_ptr<RAnimationData> animationData = std::make_shared<RAnimationData>();
        animationData->ReadFromPath(path);
        mAnimationMap[animHash] = animationData;
        return animationData;
    }
}
