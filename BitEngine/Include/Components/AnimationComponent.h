#pragma once
#include "Core/BComponent.h"
#include "Data/RMatrix4x4.h";

#include <vector>
#include <memory>


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
            void LoadAnimation(const char* animationDataPath);
            void GetAnimationChannelsMatrix(RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);
            void LoadBoneInfo(const char* boneInfoDataReferencePath);

        protected:
            void Update(float deltaTime) override;

        private:
            float mCurrentTime = 0;
            bool mLoopAnim = true;

            float GetAnimCurrentTick(float maxTime);
            std::shared_ptr<RAnimationData> pAnimationData;
            std::shared_ptr<RBoneInfoData> pMeshDataReference;
            Vector3D GetVectorKeyAtCurrentTick(std::vector<RAnimationVectorKey>& vectorKeyList);
            Quaternion GetQuatKeyAtCurrentTime(std::vector<RAnimationQuatKey>& quatKeyList);

            void ProcessBoneHierarchy(RBoneInfo& bone, RMatrix4x4 parentTransform, RMatrix4x4(&animationMatrix)[MAX_NUM_OF_ANIMATION_CHANNELS]);
        };
    }
}