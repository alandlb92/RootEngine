#include "Components/AnimationComponent.h"
#include "Graphics/AnimationManager.h"
#include "Data/BitMeshData.h"
#include "Faia/Debug.h"
#include <sstream>

void AnimationComponent::SetAnimation(const char* path)
{
    pAnimationData = AnimationManager::GetInstance()->LoadAnimationFromPath(path);
}

void AnimationComponent::GetAnimationChannelsMatrix(RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS])
{
    //Todo: review the necessity of weak ptr
    auto pa = pAnimationData.lock();

    for (size_t i = 0; i < pa->mAnimChannels.size(); ++i)
    {
        if (pa->mAnimChannels[i].mScales.size() > 0 && pa->mAnimChannels[i].mRotations.size() > 0 && pa->mAnimChannels[i].mPositions.size() > 0)
        {
            Vector3D pos = GetVectorKeyAtCurrentTime(pa->mAnimChannels[i].mPositions);
            Vector3D scale = GetVectorKeyAtCurrentTime(pa->mAnimChannels[i].mScales);
            Quaternion rot = GetQuatKeyAtCurrentTime(pa->mAnimChannels[i].mRotations);

            animationMatrix[i] = RMatrix4x4::CreateTransformationMatrix(scale, rot, pos);
        }
    }
}

void AnimationComponent::Update(float deltaTime)
{
    mCurrentTime += (deltaTime * 30);    
}

float AnimationComponent::GetLoopTime(float maxTime)
{
    int mult = mCurrentTime/maxTime;
    
    if (mult > 0)
    {
        return mCurrentTime - (maxTime * mult);
    }
    else
    {
        return mCurrentTime;
    }
}

Vector3D AnimationComponent::GetVectorKeyAtCurrentTime(std::vector<RAnimationVectorKey>& vectorKeyList)
{
    //Todo: Loop animation
    if (!mLoopAnim && mCurrentTime > vectorKeyList.back().mTime)
    {
        return vectorKeyList.back().mValue;
    }
    
    float currentTime = mCurrentTime;
    
    if (mLoopAnim)
    {
        currentTime = GetLoopTime(vectorKeyList.back().mTime);
    }



    int nextPosIdx = 0;

    for (size_t posIdx = 0; posIdx < vectorKeyList.size(); ++posIdx)
    {
        if (vectorKeyList[posIdx].mTime > currentTime)
        {
            nextPosIdx = posIdx;
            break;
        }
    }

    float previousTime = vectorKeyList[nextPosIdx - 1].mTime;
    float nextTime = vectorKeyList[nextPosIdx].mTime;
    float timeBetwen = nextTime - previousTime;
    float currTimeNormalize = currentTime - previousTime;
    float lerpValue = currTimeNormalize / timeBetwen;


    std::stringstream ss;
    ss << "animCurTime: " << currentTime << " nextTime: " << nextTime << " previousTime: " << previousTime << " lerp: " << lerpValue;
    Faia::Debug::Log(ss.str().c_str());

    return Vector3D::Lerp(vectorKeyList[nextPosIdx - 1].mValue,
        vectorKeyList[nextPosIdx].mValue, lerpValue);
}

Quaternion AnimationComponent::GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList)
{
    //Todo: Loop animation
    if (!mLoopAnim && mCurrentTime > quatKeyList.back().mTime)
    {
        return quatKeyList.back().mValue;
    }

    float currentTime = mCurrentTime;

    if (mLoopAnim)
    {
        currentTime = GetLoopTime(quatKeyList.back().mTime);
    }


    int nextPosIdx = 0;

    for (size_t posIdx = 0; posIdx < quatKeyList.size(); ++posIdx)
    {
        if (quatKeyList[posIdx].mTime > currentTime)
        {
            nextPosIdx = posIdx;
            break;
        }
    }

    float previousTime = quatKeyList[nextPosIdx - 1].mTime;
    float nextTime = quatKeyList[nextPosIdx].mTime;
    float timeBetwen = nextTime - previousTime;
    float currTimeNormalize = currentTime - previousTime;
    float lerpValue = currTimeNormalize / timeBetwen;

    return Quaternion::Lerp(quatKeyList[nextPosIdx - 1].mValue,
        quatKeyList[nextPosIdx].mValue, lerpValue);
}
