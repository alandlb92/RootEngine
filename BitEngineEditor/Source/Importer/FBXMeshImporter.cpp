#include "FBXMeshImporter.h"
#include "Data/BitMeshData.h"
#include "Data/RMatrix4x4.h"

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
            void FBXMeshImporter::Run()
            {
                RMeshData rmd;
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
                            if (rmd._boneNameToIdexMap.find(childName) != rmd._boneNameToIdexMap.end())
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
                        mState = ERROR;
                        return;
                    }
                }

                rmd.Write(outputPath);
                RMeshData rmdReadTest;
                rmdReadTest.ReadFromPath(outputPath);

                mState = DONE;
            }
        }
    }
}