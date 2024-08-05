#include "FBX/FBXMeshImporter.h"
#include "Data/BitMeshData.h"
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
            void FBXMeshImporter::Run()
            {
                RMeshData rmd;
                const char* inputPath = mArgs[0];
                const char* outputPath = mArgs[1];
                
                bool importBoneData = mArgs.size() > 2;

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

                for (int meshIndex = 0; meshIndex < aiScene->mNumMeshes; meshIndex++)
                {
                    aiMesh* aiMesh = aiScene->mMeshes[meshIndex];
                    RMeshNode mesh;

                    //Import vertices
                    for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
                    {
                        aiVector3D& vertex = aiMesh->mVertices[j];
                        Vector3D vert(vertex.x, vertex.y, vertex.z);
                        mesh.mVertices.push_back(vert);

                        aiVector3D& normal = aiMesh->mNormals[j];
                        Vector3D rnorm(normal.x, normal.y, normal.z);
                        mesh.mNormals.push_back(rnorm);
                    }

                    //Import faces
                    for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j)
                    {
                        aiFace& face = aiMesh->mFaces[j];
                        for (unsigned int k = 0; k < face.mNumIndices; ++k)
                        {
                            uint16_t index = face.mIndices[k];
                            mesh.mIndices.push_back(index);
                        }
                    }

                    //Import UV
                    if (aiMesh->HasTextureCoords(0))
                    {
                        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j)
                        {
                            aiVector3D& uv = aiMesh->mTextureCoords[0][j];
                            Vector2D uvCoord(uv.x, -uv.y);
                            mesh.mUV.push_back(uvCoord);
                        }
                    }

                    //Import Bone data                    
                    if (importBoneData)
                    {
                        const char* boneInfoRefPath = mArgs[2];
                        const RBoneInfoData rbid(boneInfoRefPath);
                        

                        mesh.mBoneData.resize(aiMesh->mNumVertices);

                        for (int j = 0; j < aiMesh->mNumBones; j++)
                        {
                            aiBone* aiBone = aiMesh->mBones[j];
                            std::string boneName(aiBone->mName.C_Str());

                            for (int wightIndex = 0; wightIndex < aiBone->mNumWeights; ++wightIndex)
                            {
                                RVertexBoneData& boneData = mesh.mBoneData[aiBone->mWeights[wightIndex].mVertexId];
                                uint32_t boneId = 0;
                                map<std::string, uint32_t>::const_iterator it = rbid.mBoneNameToIdexMap.find(boneName);

                                if (it != rbid.mBoneNameToIdexMap.end())
                                {
                                    boneId = it->second;
                                }
                                else
                                {
                                    stringstream ss;
                                    ss << "Bone index of name: " << boneName << " not found: verify if you are using the correct bone data reference";
                                    mErrorMsg = ss.str();
                                    mState = ERROR;
                                    return;
                                }

                                //Find the lowest value index and override if is bigger
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


                    mesh.mMaterialIndex = aiMesh->mMaterialIndex;
                    rmd._meshs.push_back(mesh);
                }                

                rmd.Write(outputPath);
                RMeshData rmdReadTest;
                rmdReadTest.ReadFromPath(outputPath);

                mState = DONE;
            }
        }
    }
}