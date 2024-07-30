#pragma once
#include "Core/BComponent.h"
#include <memory>
#include "Data/RMatrix4x4.h";
#include <vector>

struct RAnimationData;
struct RAnimationVectorKey;
struct RAnimationQuatKey;
struct RMeshData;
struct RBoneInfo;

class AnimationComponent : public BComponent
{
public:
    void SetAnimation(const char* path);
    void GetAnimationChannelsMatrix(RMatrix4x4 (&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);
    //Todo: separete the bone info from RMeshData, this is low down the file and will be better for implementation
    void SetBoneInfo(const char* meshDataReferencePath);

protected:
    void Update(float deltaTime) override;

private:
    float mCurrentTime = 0;
    bool mLoopAnim = true;

    float GetAnimCurrentTime(float maxTime);
    RAnimationData* pAnimationData;
    //Todo: use smart pointers
    RMeshData* pMeshDataReference;
    Vector3D GetVectorKeyAtCurrentTime(std::vector<RAnimationVectorKey>& vectorKeyList);
    Quaternion GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList);

    void ProcessBoneHierarchy(RBoneInfo& bone, RMatrix4x4 parentTransform, RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);
};