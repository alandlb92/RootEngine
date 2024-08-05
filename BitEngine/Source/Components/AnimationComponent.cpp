#include "Components/AnimationComponent.h"
#include "Graphics/AnimationManager.h"
#include "Data/BitMeshData.h"
#include "Faia/Debug.h"
#include <sstream>

namespace Faia
{
    namespace Root
    {
        void AnimationComponent::SetAnimation(const char* path)
        {
            pAnimationData = AnimationManager::GetInstance()->LoadAnimationFromPath(path);
        }

        void AnimationComponent::SetBoneInfo(const char* meshDataReferencePath)
        {
            //todo: this must load from a manager to apply flyweight
            pMeshDataReference = new RBoneInfoData();
            pMeshDataReference->ReadFromPath(meshDataReferencePath);
        }

        void AnimationComponent::GetAnimationChannelsMatrix(RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS])
        {
            //Todo: review the necessity of weak ptr
            //auto pa = pAnimationData.lock();
            ProcessBoneHierarchy(pMeshDataReference->mIndexToBoneInfo[0], RMatrix4x4::Identity(), animationMatrix);
            /*  for (int i = 0; i < pMeshDataReference->mIndexToBoneInfo.size(); i++)
              {
                  Faia::Debug::Log(pMeshDataReference->mIndexToBoneInfo[i].mName.c_str());
                  Faia::Debug::Log(animationMatrix[i].ToPrintableMatrix().c_str());
              }*/
              //Faia::Debug::Log("---------------------------One pose------------------------------");
        }

        void AnimationComponent::Update(float deltaTime)
        {
            //todo: consider ticks per second to calculate the time, the is in tiks not seconds (just multiply time * numberOfTicks)
            mCurrentTime += (deltaTime * 30);
        }

        float AnimationComponent::GetAnimCurrentTime(float maxTime)
        {
            //Todo: Loop animation
            if (!mLoopAnim && mCurrentTime > maxTime)
            {
                return maxTime;
            }

            int mult = mCurrentTime / maxTime;

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
            float currentTime = GetAnimCurrentTime(vectorKeyList.back().mTime);;

            int nextPosIdx = 0;

            for (size_t posIdx = 0; posIdx < vectorKeyList.size(); ++posIdx)
            {
                if (vectorKeyList[posIdx].mTime > currentTime)
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
            float currTimeNormalize = currentTime - previousTime;
            float lerpValue = currTimeNormalize / timeBetwen;


            std::stringstream ss;
            ss << "animCurTime: " << currentTime << " nextTime: " << nextTime << " previousTime: " << previousTime << " lerp: " << lerpValue;
            Faia::Debug::Log(ss.str().c_str());

            Vector3D previousPos = vectorKeyList[nextPosIdx - 1].mValue;
            Vector3D nextPos = vectorKeyList[nextPosIdx].mValue;

            return Vector3D::Lerp(vectorKeyList[nextPosIdx - 1].mValue,
                vectorKeyList[nextPosIdx].mValue, lerpValue);
        }

        Quaternion AnimationComponent::GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList)
        {
            float currentTime = GetAnimCurrentTime(quatKeyList.back().mTime);;
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

            return Quaternion::Lerp(quatKeyList[nextPosIdx - 1].mValue,
                quatKeyList[nextPosIdx].mValue, lerpValue);
        }

        void AnimationComponent::ProcessBoneHierarchy(RBoneInfo& bone, RMatrix4x4 parentTransform, RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS])
        {
            Vector3D scale = Vector3D(1.0f);
            Quaternion rotation = Quaternion();
            Vector3D position = Vector3D(.0f);

            if (pAnimationData->mAnimChannels.size() - 1 > bone.mIndex)
            {
                position = GetVectorKeyAtCurrentTime(pAnimationData->mAnimChannels[bone.mIndex].mPositions);
                rotation = GetQuatKeyAtCurrentTime(pAnimationData->mAnimChannels[bone.mIndex].mRotations);
                scale = GetVectorKeyAtCurrentTime(pAnimationData->mAnimChannels[bone.mIndex].mScales);
            }

            RMatrix4x4 animPoseMatrix = RMatrix4x4::CreateTransformationMatrix(position, rotation, scale);

            RMatrix4x4 globalTransformationMatrix = parentTransform * animPoseMatrix;

            animationMatrix[bone.mIndex] = (pMeshDataReference->mGlobalInverseTransform * globalTransformationMatrix * bone.mBoneOffsetMatrix).Transpose();

            /* Faia::Debug::Log(bone.mName.c_str());
             Faia::Debug::Log("mGlovalInverseTransform");
             Faia::Debug::Log(pMeshDataReference->mGlovalInverseTransform.ToPrintableMatrix().c_str());
             Faia::Debug::Log("animPoseMatrix");
             Faia::Debug::Log(animPoseMatrix.ToPrintableMatrix().c_str());
             Faia::Debug::Log("mBoneOffsetMatrix");
             Faia::Debug::Log(bone.mBoneOffsetMatrix.ToPrintableMatrix().c_str());*/

            for (int i = 0; i < bone.mChildsId.size(); ++i)
            {
                RBoneInfo& boneChild = pMeshDataReference->mIndexToBoneInfo[bone.mChildsId[i]];
                ProcessBoneHierarchy(boneChild, globalTransformationMatrix, animationMatrix);
            }
        }
    }
}