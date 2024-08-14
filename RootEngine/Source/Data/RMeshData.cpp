#include "Data/RMeshData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Faia
{
    namespace Root
    {
        void RMeshData::Write(const char* output)
        {
            std::ofstream os(output, std::ios::binary);
            if (!os) {
                stringstream ss;
                ss << "of stream cant open outPut path: " << output << std::endl;
                throw std::invalid_argument(ss.str().c_str());
            }

            uint32_t numMeshs = mRMeshNodes.size();
            os.write(reinterpret_cast<char*>(&numMeshs), sizeof(numMeshs));
            for (RMeshNode& mesh : mRMeshNodes)
            {
                uint32_t numIndices = mesh.mIndices.size();
                uint32_t numVertices = mesh.mVertices.size();
                uint32_t numUV = mesh.mUV.size();
                uint32_t numNormals = mesh.mUV.size();
                uint32_t numBoneData = mesh.mBoneData.size();

                os.write(reinterpret_cast<char*>(&numIndices), sizeof(uint32_t));
                os.write(reinterpret_cast<char*>(&numVertices), sizeof(uint32_t));
                os.write(reinterpret_cast<char*>(&numNormals), sizeof(uint32_t));
                os.write(reinterpret_cast<char*>(&numUV), sizeof(uint32_t));
                os.write(reinterpret_cast<char*>(&numBoneData), sizeof(uint32_t));

                os.write(reinterpret_cast<const char*>(mesh.mIndices.data()), numIndices * sizeof(uint16_t));
                os.write(reinterpret_cast<const char*>(mesh.mVertices.data()), numVertices * sizeof(RVector3D));
                os.write(reinterpret_cast<const char*>(mesh.mNormals.data()), numNormals * sizeof(RVector3D));
                os.write(reinterpret_cast<const char*>(mesh.mUV.data()), numUV * sizeof(RVector2D));

                if (numBoneData > 0)
                {
                    for (int i = 0; i < numBoneData; i++)
                    {
                        os.write(reinterpret_cast<const char*>(&mesh.mBoneData[i].boneId), sizeof(uint32_t) * MAX_NUM_OF_BONES_PER_VERTEX);
                        os.write(reinterpret_cast<const char*>(&mesh.mBoneData[i].weights), sizeof(float) * MAX_NUM_OF_BONES_PER_VERTEX);
                    }
                }

                os.write(reinterpret_cast<const char*>(&mesh.mMaterialIndex), sizeof(uint16_t));
            }

            os.close();
        }

        void RMeshData::ReadFromPath(const char* filePath)
        {
            std::ifstream is(filePath, std::ios::binary);

            if (!is) {
                stringstream ss;
                ss << "if stream cant open filePath path: " << filePath << std::endl;
                throw std::invalid_argument(ss.str().c_str());
            }

            uint32_t numMeshs;
            is.read(reinterpret_cast<char*>(&numMeshs), sizeof(numMeshs));

            for (uint32_t i = 0; i < numMeshs; ++i)
            {
                RMeshNode node;
                uint32_t numIndices, numVertices, numUV, numNormals, numBoneData;
                is.read(reinterpret_cast<char*>(&numIndices), sizeof(uint32_t));
                is.read(reinterpret_cast<char*>(&numVertices), sizeof(uint32_t));
                is.read(reinterpret_cast<char*>(&numNormals), sizeof(uint32_t));
                is.read(reinterpret_cast<char*>(&numUV), sizeof(uint32_t));
                is.read(reinterpret_cast<char*>(&numBoneData), sizeof(uint32_t));

                node.mIndices.resize(numIndices);
                node.mVertices.resize(numVertices);
                node.mUV.resize(numUV);
                node.mNormals.resize(numNormals);

                is.read(reinterpret_cast<char*>(node.mIndices.data()), numIndices * sizeof(uint16_t));
                is.read(reinterpret_cast<char*>(node.mVertices.data()), numVertices * sizeof(RVector3D));
                is.read(reinterpret_cast<char*>(node.mNormals.data()), numNormals * sizeof(RVector3D));
                is.read(reinterpret_cast<char*>(node.mUV.data()), numUV * sizeof(RVector2D));

                if (numBoneData > 0)
                {
                    node.mBoneData.resize(numBoneData);
                    for (int i = 0; i < numBoneData; i++)
                    {
                        is.read(reinterpret_cast<char*>(&node.mBoneData[i].boneId), sizeof(uint32_t) * MAX_NUM_OF_BONES_PER_VERTEX);
                        is.read(reinterpret_cast<char*>(&node.mBoneData[i].weights), sizeof(float) * MAX_NUM_OF_BONES_PER_VERTEX);
                    }
                }

                is.read(reinterpret_cast<char*>(&node.mMaterialIndex), sizeof(uint16_t));

                mRMeshNodes.push_back(node);
            }          

            is.close();
        }

        std::string RMeshData::ToString()
        {
            stringstream ss;
            ss << "Mesh count: " << mRMeshNodes.size() << "\n";
            for (auto m : mRMeshNodes)
            {
                ss << "MaterialIndex: " << m.mMaterialIndex << "\n";
                ss << "Vetices:\n{\n";
                for (auto v : m.mVertices)
                {
                    ss << "x: " << v.X << "\n";
                    ss << "y: " << v.Y << "\n";
                    ss << "z: " << v.Z << "\n";
                }
                ss << "\n}\n";

                ss << "Indices:\n{ ";
                for (auto i : m.mIndices)
                {
                    ss << i << ", ";
                }
                ss << " }\n";

                ss << "UV:\n{\n";
                for (auto uv : m.mUV)
                {
                    ss << "x: " << uv.X << "\n";
                    ss << "y: " << uv.Y << "\n";
                }
                ss << "\n}\n";
            }

            return ss.str();
        }

        void RAnimationData::Write(const char* output)
        {
            //std::string mName;
            //float mDuration;
            //float mTicksPerSecond;
            //std::vector<RAnimationChanel> mAnimFrames;
            //void Write(const char* output);
            //void ReadFromPath(const char* filePath);
            std::ofstream os(output, std::ios::binary);
            if (!os) {
                stringstream ss;
                ss << "of stream cant open outPut path: " << output << std::endl;
                throw std::invalid_argument(ss.str().c_str());
            }

            uint32_t nameSize = strlen(mName.c_str());
            os.write(reinterpret_cast<char*>(&nameSize), sizeof(uint32_t));
            os.write(mName.c_str(), nameSize);

            os.write(reinterpret_cast<char*>(&mDuration), sizeof(float));
            os.write(reinterpret_cast<char*>(&mTicksPerSecond), sizeof(float));

            uint32_t numOfChannels = mAnimChannels.size();
            os.write(reinterpret_cast<char*>(&numOfChannels), sizeof(uint32_t));

            for (RAnimationChannel& channel : mAnimChannels)
            {
                os.write(reinterpret_cast<char*>(&channel.mBoneId), sizeof(uint32_t));

                uint32_t mNumKeyPosition = channel.mPositions.size();
                uint32_t mNumKeyScale = channel.mScales.size();
                uint32_t mNumRotation = channel.mRotations.size();

                os.write(reinterpret_cast<char*>(&mNumKeyPosition), sizeof(uint32_t));
                os.write(reinterpret_cast<char*>(&mNumKeyScale), sizeof(uint32_t));
                os.write(reinterpret_cast<char*>(&mNumRotation), sizeof(uint32_t));

                for (RAnimationVectorKey& vecKey : channel.mPositions)
                {
                    os.write(reinterpret_cast<char*>(&vecKey.mTime), sizeof(float));
                    os.write(reinterpret_cast<char*>(&vecKey.mValue.X), sizeof(float));
                    os.write(reinterpret_cast<char*>(&vecKey.mValue.Y), sizeof(float));
                    os.write(reinterpret_cast<char*>(&vecKey.mValue.Z), sizeof(float));
                }

                for (RAnimationVectorKey& vecKey : channel.mScales)
                {
                    os.write(reinterpret_cast<char*>(&vecKey.mTime), sizeof(float));
                    os.write(reinterpret_cast<char*>(&vecKey.mValue.X), sizeof(float));
                    os.write(reinterpret_cast<char*>(&vecKey.mValue.Y), sizeof(float));
                    os.write(reinterpret_cast<char*>(&vecKey.mValue.Z), sizeof(float));
                }

                for (RAnimationQuatKey& quatKey : channel.mRotations)
                {
                    os.write(reinterpret_cast<char*>(&quatKey.mTime), sizeof(float));
                    os.write(reinterpret_cast<char*>(&quatKey.mValue.X), sizeof(float));
                    os.write(reinterpret_cast<char*>(&quatKey.mValue.Y), sizeof(float));
                    os.write(reinterpret_cast<char*>(&quatKey.mValue.Z), sizeof(float));
                    os.write(reinterpret_cast<char*>(&quatKey.mValue.W), sizeof(float));
                }
            }
        }

        void RAnimationData::ReadFromPath(const char* filePath)
        {
            std::ifstream is(filePath, std::ios::binary);

            if (!is) {
                stringstream ss;
                ss << "if stream cant open filePath path: " << filePath << std::endl;
                throw std::invalid_argument(ss.str().c_str());
            }

            uint32_t nameSize;
            is.read(reinterpret_cast<char*>(&nameSize), sizeof(uint32_t));

            char* name = new char[nameSize];
            is.read(name, nameSize);
            name[nameSize] = '\0';
            mName = name;

            is.read(reinterpret_cast<char*>(&mDuration), sizeof(float));
            is.read(reinterpret_cast<char*>(&mTicksPerSecond), sizeof(float));
            uint32_t numOfChannels;
            is.read(reinterpret_cast<char*>(&numOfChannels), sizeof(uint32_t));

            mAnimChannels = std::vector<RAnimationChannel>(numOfChannels);
            for (RAnimationChannel& channel : mAnimChannels)
            {
                uint32_t boneId;
                is.read(reinterpret_cast<char*>(&boneId), sizeof(uint32_t));
                channel.mBoneId = boneId;

                uint32_t mNumKeyPosition;
                uint32_t mNumKeyScale;
                uint32_t mNumRotation;

                is.read(reinterpret_cast<char*>(&mNumKeyPosition), sizeof(uint32_t));
                is.read(reinterpret_cast<char*>(&mNumKeyScale), sizeof(uint32_t));
                is.read(reinterpret_cast<char*>(&mNumRotation), sizeof(uint32_t));

                channel.mPositions = std::vector<RAnimationVectorKey>(mNumKeyPosition);
                channel.mScales = std::vector<RAnimationVectorKey>(mNumKeyScale);
                channel.mRotations = std::vector<RAnimationQuatKey>(mNumRotation);

                for (RAnimationVectorKey& vecKey : channel.mPositions)
                {
                    is.read(reinterpret_cast<char*>(&vecKey.mTime), sizeof(float));
                    is.read(reinterpret_cast<char*>(&vecKey.mValue.X), sizeof(float));
                    is.read(reinterpret_cast<char*>(&vecKey.mValue.Y), sizeof(float));
                    is.read(reinterpret_cast<char*>(&vecKey.mValue.Z), sizeof(float));
                }

                for (RAnimationVectorKey& vecKey : channel.mScales)
                {
                    is.read(reinterpret_cast<char*>(&vecKey.mTime), sizeof(float));
                    is.read(reinterpret_cast<char*>(&vecKey.mValue.X), sizeof(float));
                    is.read(reinterpret_cast<char*>(&vecKey.mValue.Y), sizeof(float));
                    is.read(reinterpret_cast<char*>(&vecKey.mValue.Z), sizeof(float));
                }

                for (RAnimationQuatKey& quatKey : channel.mRotations)
                {
                    is.read(reinterpret_cast<char*>(&quatKey.mTime), sizeof(float));
                    is.read(reinterpret_cast<char*>(&quatKey.mValue.X), sizeof(float));
                    is.read(reinterpret_cast<char*>(&quatKey.mValue.Y), sizeof(float));
                    is.read(reinterpret_cast<char*>(&quatKey.mValue.Z), sizeof(float));
                    is.read(reinterpret_cast<char*>(&quatKey.mValue.W), sizeof(float));
                }
            }
        }

        void RBoneInfoData::Write(const char* output)
        {
            std::ofstream os(output, std::ios::binary);
            if (!os) {
                stringstream ss;
                ss << "of stream cant open outPut path: " << output << std::endl;
                throw std::invalid_argument(ss.str().c_str());
            }

            os.write(reinterpret_cast<char*>(&mGlobalInverseTransform), sizeof(RMatrix4x4)); 

            uint32_t numBoneNameMap = mBoneNameToIdexMap.size();
            os.write(reinterpret_cast<char*>(&numBoneNameMap), sizeof(uint32_t));

            for (std::map<std::string, uint32_t>::iterator it = mBoneNameToIdexMap.begin();
                it != mBoneNameToIdexMap.end();
                ++it)
            {
                uint32_t charSize = strlen((*it).first.c_str());
                os.write(reinterpret_cast<char*>(&charSize), sizeof(uint32_t));
                os.write((*it).first.c_str(), charSize);
                os.write(reinterpret_cast<char*>(&(*it).second), sizeof(uint32_t));
            }


            for (std::map<uint32_t, RBoneInfo>::iterator it = mIndexToBoneInfo.begin();
                it != mIndexToBoneInfo.end();
                ++it)
            {
                uint32_t mapId = (*it).first;
                os.write(reinterpret_cast<char*>(&mapId), sizeof(uint32_t));

                uint32_t charSize = strlen((*it).second.mName.c_str());
                os.write(reinterpret_cast<char*>(&charSize), sizeof(uint32_t));
                os.write((*it).second.mName.c_str(), charSize);
                os.write(reinterpret_cast<char*>(&(*it).second.mIndex), sizeof(uint32_t));

                uint32_t numOfChilds = (*it).second.mChildsId.size();
                os.write(reinterpret_cast<char*>(&numOfChilds), sizeof(uint32_t));
                for (int childIndex = 0; childIndex < numOfChilds; ++childIndex)
                {
                    os.write(reinterpret_cast<char*>(&(*it).second.mChildsId[childIndex]), sizeof(uint32_t));
                }

                os.write(reinterpret_cast<char*>(&(*it).second.mBoneOffsetMatrix), sizeof(RMatrix4x4));
                os.write(reinterpret_cast<char*>(&(*it).second.mNodeTransformationMatrix), sizeof(RMatrix4x4));
            }
        }

        void RBoneInfoData::ReadFromPath(const char* filePath)
        {
            std::ifstream is(filePath, std::ios::binary);

            if (!is) {
                stringstream ss;
                ss << "if stream cant open filePath path: " << filePath << std::endl;
                throw std::invalid_argument(ss.str().c_str());
            }

            is.read(reinterpret_cast<char*>(&mGlobalInverseTransform), sizeof(RMatrix4x4));  uint32_t numBoneNameMap;

            is.read(reinterpret_cast<char*>(&numBoneNameMap), sizeof(uint32_t));

            if (numBoneNameMap)
            {
                for (uint32_t i = 0; i < numBoneNameMap; ++i)
                {
                    uint32_t charSize;
                    is.read(reinterpret_cast<char*>(&charSize), sizeof(uint32_t));

                    char* boneName = new char[charSize];
                    is.read(boneName, charSize);
                    boneName[charSize] = '\0';

                    uint32_t boneId;
                    is.read(reinterpret_cast<char*>(&boneId), sizeof(uint32_t));

                    mBoneNameToIdexMap[boneName] = boneId;
                }

                for (uint32_t i = 0; i < numBoneNameMap; ++i)
                {
                    uint32_t mapId, boneIndex;
                    int32_t parentIndex;
                    is.read(reinterpret_cast<char*>(&mapId), sizeof(uint32_t));
                    uint32_t charSize;
                    is.read(reinterpret_cast<char*>(&charSize), sizeof(uint32_t));
                    char* boneName = new char[charSize];
                    RMatrix4x4 offsetMatrix;
                    RMatrix4x4 transformationMatrix;

                    is.read(boneName, charSize);
                    boneName[charSize] = '\0';
                    is.read(reinterpret_cast<char*>(&boneIndex), sizeof(uint32_t));

                    uint32_t numOfChilds;
                    is.read(reinterpret_cast<char*>(&numOfChilds), sizeof(uint32_t));


                    std::vector<uint32_t> childs(numOfChilds);
                    for (int childIndex = 0; childIndex < numOfChilds; ++childIndex)
                    {
                        is.read(reinterpret_cast<char*>(&childs[childIndex]), sizeof(uint32_t));
                    }

                    is.read(reinterpret_cast<char*>(&offsetMatrix), sizeof(RMatrix4x4));
                    is.read(reinterpret_cast<char*>(&transformationMatrix), sizeof(RMatrix4x4));

                    mIndexToBoneInfo[mapId] = RBoneInfo{ std::string(boneName), boneIndex, childs, offsetMatrix, transformationMatrix };
                }
            }
        }
    }
}