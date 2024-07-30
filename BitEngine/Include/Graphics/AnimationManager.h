#pragma once
#include <ostream>
#include <unordered_map>
#include <memory>

struct RAnimationData;

class AnimationManager
{
public:
    static AnimationManager* GetInstance()
    {
        if (sInstance == nullptr)
        {
            sInstance = std::make_unique<AnimationManager>();
        }

        return sInstance.get();
    };

    RAnimationData* LoadAnimationFromPath(const char* path);

private:
	static std::unique_ptr<AnimationManager> sInstance;
	std::unordered_map<uint32_t, std::shared_ptr<RAnimationData>> mAnimationMap;

};