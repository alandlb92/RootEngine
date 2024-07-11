#include "FBXImporter.h"
#include "Data/BitMeshData.h"
#include <iostream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

                //this is slow to load, we need to convert to a better file
                const aiScene* aiScene = importer.ReadFile(pFile,
                    aiProcess_CalcTangentSpace |
                    aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_SortByPType);

                if (aiScene == nullptr)
                {
                    //error to import
                    stringstream ss;
                    ss << "error to import: " << inputPath;
                    _state = ERROR;
                    mErrorMsg = ss.str();
                    return;
                }

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

                        for (size_t j = 0; j < aiMesh->mNumBones; j++)
                        {
                            aiBone* aiBone = aiMesh->mBones[j];


                            for (rsize_t wightIndex = 0; wightIndex < aiBone->mNumWeights; ++wightIndex)
                            {
                                RVertexBoneData& boneData = mesh._boneData[aiBone->mWeights[wightIndex].mVertexId];
                                uint32_t boneId = 0;
                                std::string boneName(aiBone->mName.C_Str());

                                if (rmd._boneNameToIdexMap.find(boneName) == rmd._boneNameToIdexMap.end())
                                {
                                    boneId = rmd._boneNameToIdexMap.size();
                                    rmd._boneNameToIdexMap[boneName] = boneId;
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

                rmd.Write(outputPath);

                _state = DONE;
            }

            void FBXImporter::ImportAnimationAsync()
            {
                Assimp::Importer importer;
                std::string pFile(inputPath);

                //this is slow to load, we need to convert to a better file
                const aiScene* aiScene = importer.ReadFile(pFile,
                    aiProcess_CalcTangentSpace |
                    aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_SortByPType);

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

                for (int channelId = 0; channelId < anim->mNumChannels; ++channelId)
                {
                    RAnimationChannel ac;
                    aiNodeAnim* nodeAnim = anim->mChannels[channelId];
                    std::string nodeName = std::string(nodeAnim->mNodeName.C_Str());
                    auto it = meshReference._boneNameToIdexMap.find(nodeName);

                    if (it == meshReference._boneNameToIdexMap.end())
                    {
                        stringstream ss;
                        ss << "The bone name " << nodeName << "was not found, this mesh reference dosent match with the animation chanells";
                        _state = ERROR;
                        mErrorMsg = ss.str();
                        return;
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
                    }

                    rad.mAnimChannels.push_back(ac);
                }

                rad.Write(outputPath);

                _state = DONE;
            }
        }
    }
}