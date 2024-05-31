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

            void FBXImporter::Run()
            {
                if (inputPath != "" && outputPath != "")
                {
                    _asyncResult = std::async(std::launch::async, &FBXImporter::ImportMeshAsync, this);
                    _state = RUNNING;
                }
                else
                {
                    throw std::invalid_argument("inputPath and outputPath cant be empty");
                    _state = ERROR;
                }
            }

            FBXImporter::State FBXImporter::GetState()
            {
                return _state;
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
                    ss << "error to import: " << inputPath << std::endl;
                    _state = ERROR;
                    throw std::invalid_argument(ss.str().c_str());
                    return;
                }

                for (int i = 0; i < aiScene->mNumMeshes; i++)
                {
                    aiMesh* aiMesh = aiScene->mMeshes[i];
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
                        for (size_t j = 0; j < aiMesh->mNumBones; j++)
                        {
                            aiBone* aiBone = aiMesh->mBones[j];
                            RBone rBone;
                            rBone._boneName = aiBone->mName.C_Str();
                            for (rsize_t k = 0; k < aiBone->mNumWeights; ++k)
                            {
                                RVertexWeightData rVertexData;
                                rVertexData.vertexId = aiBone->mWeights[k].mVertexId;
                                rVertexData.weight = aiBone->mWeights[k].mWeight;
                                rBone._weights.push_back(rVertexData);
                            }

                            mesh._skeletonData._bones.push_back(rBone);
                        }
                    }

                    mesh._materialIndex = aiMesh->mMaterialIndex;
                    rmd._meshs.push_back(mesh);
                }


                rmd.Write(outputPath);

                _state = DONE;
            }           
        }
    }
}