#pragma once
#include "Core/BComponent.h"
#include <memory>
#include "Data/RMatrix4x4.h";
#include <vector>

namespace Faia
{
    namespace Root
    {
        struct RAnimationData;
        struct RAnimationVectorKey;
        struct RAnimationQuatKey;
        struct RBoneInfoData;
        struct RBoneInfo;

        class AnimationComponent : public BComponent
        {
        public:
            void SetAnimation(const char* path);
            void GetAnimationChannelsMatrix(RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);
            void SetBoneInfo(const char* boneInfoDataReferencePath);

        protected:
            void Update(float deltaTime) override;

        private:
            float mCurrentTime = 0;
            bool mLoopAnim = true;

            float GetAnimCurrentTime(float maxTime);
            RAnimationData* pAnimationData;
            //Todo: use smart pointers
            RBoneInfoData* pMeshDataReference;
            Vector3D GetVectorKeyAtCurrentTime(std::vector<RAnimationVectorKey>& vectorKeyList);
            Quaternion GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList);

            void ProcessBoneHierarchy(RBoneInfo& bone, RMatrix4x4 parentTransform, RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);
        };
    }
}