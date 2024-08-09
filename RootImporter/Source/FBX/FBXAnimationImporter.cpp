
#include "FBX/FBXAnimationImporter.h"

#include "Data/RMeshData.h"
#include "Data/RMatrix4x4.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>

namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            void FBXAnimationImporter::Run()
            {
                const char* inputPath = mArgs[0];
                const char* outputPath = mArgs[1];
                const char* inputRef = mArgs[2];

                unsigned flags = aiProcess_CalcTangentSpace |
                    aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_SortByPType |
                    aiProcess_PopulateArmatureData;

                aiPropertyStore* aiprops = aiCreatePropertyStore();
                aiSetImportPropertyInteger(aiprops, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
                const aiScene* aiScene = aiImportFileExWithProperties(inputPath, flags, nullptr, aiprops);

                if (aiScene == nullptr)
                {
                    //error to import
                    stringstream ss;
                    ss << "error to import: " << inputPath;
                    mState = ERROR;
                    mErrorMsg = ss.str();
                    return;
                }

                if (inputRef == nullptr)
                {
                    stringstream ss;
                    ss << "An argument is missing, to import animation you need to reference a mesh with bones";
                    mState = ERROR;
                    mErrorMsg = ss.str();
                    return;
                }

                //Import Animation
                //TODO: This will just import the animation 0, we need to import all animation and do some editor in the future
                if (aiScene->mNumAnimations == 0)
                {
                    //error to import
                    stringstream ss;
                    ss << "There is no animations in this arquive: " << inputPath;
                    mState = ERROR;
                    mErrorMsg = ss.str();
                    return;
                }

                const RBoneInfoData boneInfoReference(inputRef);

                aiAnimation* anim = aiScene->mAnimations[0];

                RAnimationData rad;
                rad.mName = std::string(anim->mName.C_Str());
                rad.mDuration = static_cast<float>(anim->mDuration);
                rad.mTicksPerSecond = static_cast<float>(anim->mTicksPerSecond);

                rad.mAnimChannels = std::vector<RAnimationChannel>(anim->mNumChannels);

                for (int channelId = 0; channelId < anim->mNumChannels; ++channelId)
                {
                    RAnimationChannel ac;
                    aiNodeAnim* nodeAnim = anim->mChannels[channelId];
                    std::string nodeName = std::string(nodeAnim->mNodeName.C_Str());
                    auto it = boneInfoReference.mBoneNameToIdexMap.find(nodeName);

                    if (it == boneInfoReference.mBoneNameToIdexMap.end())
                    {
                        std::cout << "\033[1;33mWarning Bone: " << nodeName << " Not found in mesh reference!\033[0m" << std::endl;
                        continue;
                    }
                    else
                    {
                        ac.mBoneId = it->second;

                        for (int keyIndex = 0; keyIndex < nodeAnim->mNumPositionKeys; ++keyIndex)
                        {
                            aiVectorKey& vectorKey = nodeAnim->mPositionKeys[keyIndex];
                            ac.mPositions.push_back({ static_cast<float>(vectorKey.mTime), Vector3D(vectorKey.mValue.x, vectorKey.mValue.y, vectorKey.mValue.z) });
                        }

                        for (int keyIndex = 0; keyIndex < nodeAnim->mNumScalingKeys; ++keyIndex)
                        {
                            aiVectorKey& vectorKey = nodeAnim->mScalingKeys[keyIndex];
                            ac.mScales.push_back({ static_cast<float>(vectorKey.mTime), Vector3D(vectorKey.mValue.x, vectorKey.mValue.y, vectorKey.mValue.z) });
                        }

                        for (int keyIndex = 0; keyIndex < nodeAnim->mNumRotationKeys; ++keyIndex)
                        {
                            aiQuatKey& quatKey = nodeAnim->mRotationKeys[keyIndex];
                            ac.mRotations.push_back({ static_cast<float>(quatKey.mTime), Quaternion(quatKey.mValue.x, quatKey.mValue.y, quatKey.mValue.z, quatKey.mValue.w) });
                        }

                        rad.mAnimChannels[ac.mBoneId] = ac;
                    }

                }

                rad.Write(outputPath);
                RAnimationData radTest;
                radTest.ReadFromPath(outputPath);

                mState = DONE;
            }
        }
    }
}