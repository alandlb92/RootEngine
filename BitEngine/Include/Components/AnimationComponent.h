#pragma once
#include "Core/BComponent.h"
#include <memory>
#include "Data/RMatrix4x4.h";
#include <vector>

struct RAnimationData;
struct RAnimationVectorKey;
struct RAnimationQuatKey;

class AnimationComponent : public BComponent
{
public:
    void SetAnimation(const char* path);
    void GetAnimationChannelsMatrix(RMatrix4x4 (&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);

protected:
    void Update(float deltaTime) override;

private:
    float mCurrentTime = 0;
    bool mLoopAnim = true;

    float GetAnimCurrentTime(float maxTime);
    std::weak_ptr<RAnimationData> pAnimationData;    
    Vector3D GetVectorKeyAtCurrentTime(std::vector<RAnimationVectorKey>& vectorKeyList);
    Quaternion GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList);
};