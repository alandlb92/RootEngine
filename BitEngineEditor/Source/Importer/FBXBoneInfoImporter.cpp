#include "FBXBoneInfoImporter.h"

#include "Data/BitMeshData.h"

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
            void FBXBoneInfoImporter::Run()
            {
                RBoneInfoData rbid;
                const char* inputPath = mArgs[0];
                const char* outputPath = mArgs[1];

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
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                rbid.mGlovalInverseTransform = AiMatrixToRMatrix(aiScene->mRootNode->mTransformation.Inverse());
                std::map<uint32_t, aiBone*> boneIndexToAiBone;

                for (int meshIndex = 0; meshIndex < aiScene->mNumMeshes; meshIndex++)
                {
                    aiMesh* aiMesh = aiScene->mMeshes[meshIndex];

                    for (int j = 0; j < aiMesh->mNumBones; j++)
                    {
                        aiBone* aiBone = aiMesh->mBones[j];
                        std::string boneName(aiBone->mName.C_Str());

                        for (int wightIndex = 0; wightIndex < aiBone->mNumWeights; ++wightIndex)
                        {
                            if (rbid.mBoneNameToIdexMap.find(boneName) == rbid.mBoneNameToIdexMap.end())
                            {
                                uint32_t boneId = rbid.mBoneNameToIdexMap.size();
                                rbid.mBoneNameToIdexMap[boneName] = boneId;
                                boneIndexToAiBone[boneId] = aiBone;
                            }
                        }
                    }
                }


                for (auto& map : rbid.mBoneNameToIdexMap)
                {
                    std::string boneName = map.first;
                    uint32_t index = map.second;
                    aiNode* boneNode = boneIndexToAiBone[index]->mNode;

                    if (boneNode)
                    {
                        std::vector<uint32_t> childs;
                        std::string assimpSeparator = "_$";

                        for (unsigned i = 0; i < boneNode->mNumChildren; ++i)
                        {
                            std::string childName(boneNode->mChildren[i]->mName.C_Str());
                            if (rbid.mBoneNameToIdexMap.find(childName) != rbid.mBoneNameToIdexMap.end())
                                childs.push_back(rbid.mBoneNameToIdexMap[childName]);
                        }

                        RMatrix4x4 offsetMatrix = AiMatrixToRMatrix(boneIndexToAiBone[index]->mOffsetMatrix);
                        RMatrix4x4 transformMatrix = AiMatrixToRMatrix(boneNode->mTransformation);
                        rbid.mIndexToBoneInfo[index] = RBoneInfo{ boneName, index, childs, offsetMatrix, transformMatrix };
                    }
                    else
                    {
                        stringstream ss;
                        ss << "Bone node of name: : " << boneName << " not found!";
                        mErrorMsg = ss.str();
                        mState = ERROR;
                        return;
                    }
                }

                rbid.Write(outputPath);
                RBoneInfoData rbidReadTest;
                rbidReadTest.ReadFromPath(outputPath);

                mState = DONE;

            }//end run function
        }//namespace Importer
    }//namespace Root
}//namespace Faia