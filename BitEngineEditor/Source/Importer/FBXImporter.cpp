#include "FBXImporter.h"
#include "Data/BitMeshData.h"
#include <iostream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>

namespace Faia
{
    namespace BitEngineEditor
    {
        namespace Importer
        {

            FBXImporter::FBXImporter()
            {
                inputPath = "";
                outputPath = "";
                _state = WAITING_START;
            }

            void FBXImporter::Run(const std::string& commandType)
            {
                bool error = false;
                if (inputPath != "" && outputPath != "")
                {
                    ImporterType importerType = NOT_FOUND;
                    auto it = s_typeMap.find(commandType);
                    if (it != s_typeMap.end())
                    {
                        importerType = it->second;
                    }

                    switch (importerType)
                    {
                    case MS:
                        _asyncResult = std::async(std::launch::async, &FBXImporter::ImportMeshAsync, this);
                        break;

                    case ANIM:
                        _asyncResult = std::async(std::launch::async, &FBXImporter::ImportAnimationAsync, this);
                        break;
                    default:
                        error = true;
                        break;
                    }
                    _state = RUNNING;
                }
                else
                {
                    error = true;
                }

                if (error)
                {
                    mErrorMsg = "inputPath and outputPath cant be empty";
                    _state = ERROR;
                }
            }

            FBXImporter::State FBXImporter::GetState()
            {
                return _state;
            }

            std::string FBXImporter::GetErrorMsg()
            {
                return mErrorMsg;
            }

            void FBXImporter::ImportMeshAsync()
            {
                RMeshData rmd;
                Assimp::Importer importer;
                std::string pFile(inputPath);

                unsigned flags = aiProcess_CalcTangentSpace |
                                 aiProcess_Triangulate |
                                 aiProcess_JoinIdenticalVertices |
                                 aiProcess_SortByPType |
                                 aiProcess_PopulateArmatureData;

                aiPropertyStore* aiprops = aiCreatePropertyStore();
                aiSetImportPropertyInteger(aiprops, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
                const aiScene* aiScene = aiImportFileExWithProperties(inputPath, flags, nullptr, aiprops);

                //this is slow to load, we need to convert to a better file
               /* const aiScene* aiScene = importer.ReadFile(pFile,
                    aiProcess_CalcTangentSpace |
                    aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_SortByPType |
                    aiProcess_PopulateArmatureData);*/

                if (aiScene == nullptr)
                {
                    //error to import
                    stringstream ss;
                    ss << "error to import: " << inputPath;
                    mErrorMsg = ss.str();
                    _state = ERROR;
                    return;
                }

                rmd.mGlovalInverseTransform = AiMatrixToRMatrix(aiScene->mRootNode->mTransformation.Inverse());

                std::map<uint32_t, aiBone*> boneIndexToAiBone;

                for (int meshIndex = 0; meshIndex < aiScene->mNumMeshes; meshIndex++)
                {
                    aiMesh* aiMesh = aiScene->mMeshes[meshIndex];
                    RMeshNode mesh;

                    //Import vertices
                    for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
                    {
                        aiVector3D& vertex = aiMesh->mVertices[j];
                        Vector3D vert(vertex.x, vertex.y, vertex.z);
                        mesh._vertices.push_back(vert);

                        aiVector3D& normal = aiMesh->mNormals[j];
                        Vector3D rnorm(normal.x, normal.y, normal.z);
                        mesh._normals.push_back(rnorm);
                    }

                    //Import faces
                    for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j)
                    {
                        aiFace& face = aiMesh->mFaces[j];
                        for (unsigned int k = 0; k < face.mNumIndices; ++k)
                        {
                            uint16_t index = face.mIndices[k];
                            mesh._indices.push_back(index);
                        }
                    }

                    //Import UV
                    if (aiMesh->HasTextureCoords(0))
                    {
                        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
                        {
                            aiVector3D& uv = aiMesh->mTextureCoords[0][j];
                            Vector2D uvCoord(uv.x, -uv.y);
                            mesh._uv.push_back(uvCoord);
                        }
                    }

                    //Import Bone                    
                    if (aiMesh->mNumBones > 0)
                    {
                        mesh._boneData.resize(aiMesh->mNumVertices);

                        for (int j = 0; j < aiMesh->mNumBones; j++)
                        {
                            aiBone* aiBone = aiMesh->mBones[j];
                            std::string boneName(aiBone->mName.C_Str());

                            for (int wightIndex = 0; wightIndex < aiBone->mNumWeights; ++wightIndex)
                            {
                                RVertexBoneData& boneData = mesh._boneData[aiBone->mWeights[wightIndex].mVertexId];
                                uint32_t boneId = 0;

                                if (rmd._boneNameToIdexMap.find(boneName) == rmd._boneNameToIdexMap.end())
                                {
                                    boneId = rmd._boneNameToIdexMap.size();
                                    rmd._boneNameToIdexMap[boneName] = boneId;
                                    boneIndexToAiBone[boneId] = aiBone;
                                }
                                else
                                {
                                    boneId = rmd._boneNameToIdexMap[boneName];
                                }

                                bool moreThan4Bones = true;

                                //Find the lowest value index
                                float minValue = 2.0f;
                                int minValueIndex = 0;

                                for (size_t i = 0; i < MAX_NUM_OF_BONES_PER_VERTEX; ++i)
                                {
                                    if (boneData.weights[i] < minValue)
                                    {
                                        minValue = boneData.weights[i];
                                        minValueIndex = i;
                                    }
                                }

                                if (minValue < aiBone->mWeights[wightIndex].mWeight)
                                {
                                    boneData.boneId[minValueIndex] = boneId;
                                    boneData.weights[minValueIndex] = aiBone->mWeights[wightIndex].mWeight;
                                }
                            }
                        }
                    }


                    mesh._materialIndex = aiMesh->mMaterialIndex;
                    rmd._meshs.push_back(mesh);
                }

                //Create bone infos
                bool alreadyHasRoot = false;
                for (auto& map : rmd._boneNameToIdexMap)
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
                            if(rmd._boneNameToIdexMap.find(childName) != rmd._boneNameToIdexMap.end())
                                childs.push_back(rmd._boneNameToIdexMap[childName]);
                        }

                        RMatrix4x4 offsetMatrix = AiMatrixToRMatrix(boneIndexToAiBone[index]->mOffsetMatrix);
                        RMatrix4x4 transformMatrix = AiMatrixToRMatrix(boneNode->mTransformation);
                        rmd.mIndexToBoneInfo[index] = RBoneInfo{ boneName, index, childs, offsetMatrix, transformMatrix };
                    }
                    else
                    {
                        stringstream ss;
                        ss << "Bone node of name: : " << boneName << " not found!";
                        mErrorMsg = ss.str();
                        _state = ERROR;
                        return;
                    }
                }

                rmd.Write(outputPath);
                RMeshData rmdReadTest;
                rmdReadTest.ReadFromPath(outputPath);

                _state = DONE;
            }

            RMatrix4x4 FBXImporter::AiMatrixToRMatrix(aiMatrix4x4 assimpMatrix)
            {
                RMatrix4x4 result;

                result.m00 = assimpMatrix.a1; result.m01 = assimpMatrix.a2; result.m02 = assimpMatrix.a3; result.m03 = assimpMatrix.a4;
                result.m10 = assimpMatrix.b1; result.m11 = assimpMatrix.b2; result.m12 = assimpMatrix.b3; result.m13 = assimpMatrix.b4;
                result.m20 = assimpMatrix.c1; result.m21 = assimpMatrix.c2; result.m22 = assimpMatrix.c3; result.m23 = assimpMatrix.c4;
                result.m30 = assimpMatrix.d1; result.m31 = assimpMatrix.d2; result.m32 = assimpMatrix.d3; result.m33 = assimpMatrix.d4;

                return result;
            }

            void FBXImporter::ImportAnimationAsync()
            {
                Assimp::Importer importer;
                std::string pFile(inputPath);

                /*const aiScene* aiScene = importer.ReadFile(pFile,
                    aiProcess_CalcTangentSpace |
                    aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_SortByPType);*/
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
                    _state = ERROR;
                    mErrorMsg = ss.str();
                    return;
                }

                if (inputRef == nullptr)
                {
                    stringstream ss;
                    ss << "An argument is missing, to import animation you need to reference a mesh with bones";
                    _state = ERROR;
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
                    _state = ERROR;
                    mErrorMsg = ss.str();
                    return;
                }

                RMeshData meshReference;
                meshReference.ReadFromPath(inputRef);

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
                    auto it = meshReference._boneNameToIdexMap.find(nodeName);

                    if (it == meshReference._boneNameToIdexMap.end())
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

                std::vector<BoneNode> rootNode = std::vector<BoneNode>(meshReference._boneNameToIdexMap.size());

                rad.Write(outputPath);

                _state = DONE;
            }



        }
    }
}