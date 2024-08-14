#include "Components/RAnimationComponent.h"
#include "Faia/Debug.h"
#include "Core/ResourcesManager.h"

namespace Faia
{
    namespace Root
    {
        void RAnimationComponent::LoadAnimation(const char* animationDataPath)
        {
            pAnimationData = GetResourcesManager()->Load<RAnimationData>(animationDataPath);
        }

        void RAnimationComponent::LoadBoneInfo(const char* boneInfoDataReferencePath)
        {
            pMeshDataReference = GetResourcesManager()->Load<RBoneInfoData>(boneInfoDataReferencePath);
        }

        void RAnimationComponent::GetAnimationChannelsMatrix(RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS])
        {
            ProcessBoneHierarchy(pMeshDataReference->mIndexToBoneInfo[0], RMatrix4x4::Identity(), animationMatrix);
        }

        void RAnimationComponent::Update(float deltaTime)
        {
            mCurrentTime += deltaTime;
        }

        float RAnimationComponent::GetAnimCurrentTick(float maxTick)
        {
            float currentTick = mCurrentTime * pAnimationData->mTicksPerSecond;

            if (!mLoopAnim && currentTick > maxTick)
            {
                return maxTick;
            }

            int mult = currentTick / maxTick;

            if (mult > 0)
            {
                return currentTick - (maxTick * mult);
            }
            else
            {
                return currentTick;
            }
        }

        RVector3D RAnimationComponent::GetVectorKeyAtCurrentTick(std::vector<RAnimationVectorKey>& vectorKeyList)
        {
            float currentTick = GetAnimCurrentTick(vectorKeyList.back().mTime);;

            int nextPosIdx = 0;

            for (size_t posIdx = 0; posIdx < vectorKeyList.size(); ++posIdx)
            {
                if (vectorKeyList[posIdx].mTime > currentTick)
                {
                    nextPosIdx = posIdx;
                    break;
                }
            }

            if (nextPosIdx == 0)
            {
                return vectorKeyList.back().mValue;
            }

            float previousTime = vectorKeyList[nextPosIdx - 1].mTime;
            float nextTime = vectorKeyList[nextPosIdx].mTime;
            float timeBetwen = nextTime - previousTime;
            float currTimeNormalize = currentTick - previousTime;
            float lerpValue = currTimeNormalize / timeBetwen;


            RVector3D previousPos = vectorKeyList[nextPosIdx - 1].mValue;
            RVector3D nextPos = vectorKeyList[nextPosIdx].mValue;

            return RVector3D::Lerp(vectorKeyList[nextPosIdx - 1].mValue,
                vectorKeyList[nextPosIdx].mValue, lerpValue);
        }

        RQuaternion RAnimationComponent::GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList)
        {
            float currentTime = GetAnimCurrentTick(quatKeyList.back().mTime);;
            int nextPosIdx = 0;

            for (size_t posIdx = 0; posIdx < quatKeyList.size(); ++posIdx)
            {
                if (quatKeyList[posIdx].mTime > currentTime)
                {
                    nextPosIdx = posIdx;
                    break;
                }
            }

            if (nextPosIdx == 0)
            {
                return quatKeyList.back().mValue;
            }

            float previousTime = quatKeyList[nextPosIdx - 1].mTime;
            float nextTime = quatKeyList[nextPosIdx].mTime;
            float timeBetwen = nextTime - previousTime;
            float currTimeNormalize = currentTime - previousTime;
            float lerpValue = currTimeNormalize / timeBetwen;

            return RQuaternion::Lerp(quatKeyList[nextPosIdx - 1].mValue,
                quatKeyList[nextPosIdx].mValue, lerpValue);
        }

        void RAnimationComponent::ProcessBoneHierarchy(RBoneInfo& bone, RMatrix4x4 parentTransform, RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS])
        {
            RVector3D scale = RVector3D(1.0f);
            RQuaternion rotation = RQuaternion();
            RVector3D position = RVector3D(.0f);

            if (pAnimationData->mAnimChannels.size() - 1 > bone.mIndex)
            {
                position = GetVectorKeyAtCurrentTick(pAnimationData->mAnimChannels[bone.mIndex].mPositions);
                rotation = GetQuatKeyAtCurrentTime(pAnimationData->mAnimChannels[bone.mIndex].mRotations);
                scale = GetVectorKeyAtCurrentTick(pAnimationData->mAnimChannels[bone.mIndex].mScales);
            }

            RMatrix4x4 animPoseMatrix = RMatrix4x4::CreateTransformationMatrix(position, rotation, scale);

            RMatrix4x4 globalTransformationMatrix = parentTransform * animPoseMatrix;

            animationMatrix[bone.mIndex] = (pMeshDataReference->mGlobalInverseTransform * globalTransformationMatrix * bone.mBoneOffsetMatrix).Transpose();

            for (int i = 0; i < bone.mChildsId.size(); ++i)
            {
                RBoneInfo& boneChild = pMeshDataReference->mIndexToBoneInfo[bone.mChildsId[i]];
                ProcessBoneHierarchy(boneChild, globalTransformationMatrix, animationMatrix);
            }
        }
    }
}