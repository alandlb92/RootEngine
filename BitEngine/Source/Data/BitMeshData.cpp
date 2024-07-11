#include "Data/BitMeshData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


void RMeshData::Write(const char* output)
{
    std::ofstream os(output, std::ios::binary);
    if (!os) {
        stringstream ss;
        ss << "of stream cant open outPut path: " << output << std::endl;
        throw std::invalid_argument(ss.str().c_str());
    }

    uint32_t numMeshs = _meshs.size();
    os.write(reinterpret_cast<char*>(&numMeshs), sizeof(numMeshs));
    for (RMeshNode& mesh : _meshs)
    {
        uint32_t numIndices = mesh._indices.size();
        uint32_t numVertices = mesh._vertices.size();
        uint32_t numUV = mesh._uv.size();
        uint32_t numNormals = mesh._uv.size();
        uint32_t numBoneData = mesh._boneData.size();

        os.write(reinterpret_cast<char*>(&numIndices), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numVertices), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numNormals), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numUV), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numBoneData), sizeof(uint32_t));

        os.write(reinterpret_cast<const char*>(mesh._indices.data()), numIndices * sizeof(uint16_t));
        os.write(reinterpret_cast<const char*>(mesh._vertices.data()), numVertices * sizeof(Vector3D));
        os.write(reinterpret_cast<const char*>(mesh._normals.data()), numNormals * sizeof(Vector3D));
        os.write(reinterpret_cast<const char*>(mesh._uv.data()), numUV * sizeof(Vector2D));

        if (numBoneData > 0)
        {
            for (int i = 0; i < numBoneData; i++)
            {
                os.write(reinterpret_cast<const char*>(&mesh._boneData[i].boneId), sizeof(uint32_t) * MAX_NUM_OF_BONES_PER_VERTEX);
                os.write(reinterpret_cast<const char*>(&mesh._boneData[i].weights), sizeof(float) * MAX_NUM_OF_BONES_PER_VERTEX);
            }
        }

        os.write(reinterpret_cast<const char*>(&mesh._materialIndex), sizeof(uint16_t));        
    }

    uint32_t numBoneNameMap = _boneNameToIdexMap.size();
    os.write(reinterpret_cast<char*>(&numBoneNameMap), sizeof(uint32_t));

    if (numBoneNameMap)
    {
        for (std::map<std::string, uint32_t>::iterator it = _boneNameToIdexMap.begin();
            it != _boneNameToIdexMap.end();
            ++it)
        {
            uint32_t charSize = strlen((*it).first.c_str());
            os.write(reinterpret_cast<char*>(&charSize), sizeof(uint32_t));
            os.write((*it).first.c_str(), charSize);
            os.write(reinterpret_cast<char*>(&(*it).second), sizeof(uint32_t));
        }
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

        node._indices.resize(numIndices);
        node._vertices.resize(numVertices);
        node._uv.resize(numUV);
        node._normals.resize(numNormals);

        is.read(reinterpret_cast<char*>(node._indices.data()), numIndices * sizeof(uint16_t));
        is.read(reinterpret_cast<char*>(node._vertices.data()), numVertices * sizeof(Vector3D));
        is.read(reinterpret_cast<char*>(node._normals.data()), numNormals * sizeof(Vector3D));
        is.read(reinterpret_cast<char*>(node._uv.data()), numUV * sizeof(Vector2D));
        
        if (numBoneData > 0)
        {
            node._boneData.resize(numBoneData);
            for (int i = 0; i < numBoneData; i++)
            {
                is.read(reinterpret_cast<char*>(&node._boneData[i].boneId), sizeof(uint32_t) * MAX_NUM_OF_BONES_PER_VERTEX);
                is.read(reinterpret_cast<char*>(&node._boneData[i].weights), sizeof(float) * MAX_NUM_OF_BONES_PER_VERTEX);
            }
        }
        
        is.read(reinterpret_cast<char*>(&node._materialIndex), sizeof(uint16_t));
        
        _meshs.push_back(node);
    }

    uint32_t numBoneNameMap;
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

            _boneNameToIdexMap[boneName] = boneId;
        }
    }

    is.close();
}

std::string RMeshData::ToString()
{
    stringstream ss;
    ss << "Mesh count: " << _meshs.size() << "\n";
    for (auto m : _meshs)
    {
        ss << "MaterialIndex: " << m._materialIndex << "\n";
        ss << "Vetices:\n{\n";
        for (auto v : m._vertices)
        {
            ss << "x: " << v.X << "\n";
            ss << "y: " << v.Y << "\n";
            ss << "z: " << v.Z << "\n";
        }
        ss << "\n}\n";

        ss << "Indices:\n{ ";
        for (auto i : m._indices)
        {
            ss << i << ", ";
        }
        ss << " }\n";

        ss << "UV:\n{\n";
        for (auto uv : m._uv)
        {
            ss << "x: " << uv.X << "\n";
            ss << "y: " << uv.Y << "\n";
        }
        ss << "\n}\n";
    }

    return ss.str();
}


void RSkeletonData::Write(std::ofstream& os)
{
    uint32_t numBones = _bones.size();
    os.write(reinterpret_cast<char*>(&numBones), sizeof(numBones));
    for (auto& bone : _bones)
    {
        uint32_t namesize = std::strlen(bone._boneName);

        os.write(reinterpret_cast<char*>(&namesize), sizeof(uint32_t));
        os.write(bone._boneName, namesize);

        uint32_t numOfWeights = bone._weights.size();
        os.write(reinterpret_cast<char*>(&numOfWeights), sizeof(uint32_t));

        os.write(reinterpret_cast<const char*>(bone._weights.data()), numOfWeights * sizeof(RVertexWeightData));
    }
}

void RSkeletonData::Read(std::ifstream& is)
{
    uint32_t numOfBones;
    is.read(reinterpret_cast<char*>(&numOfBones), sizeof(numOfBones));

    for (uint32_t i = 0; i < numOfBones; ++i)
    {
        RBone bone;

        uint32_t namesize;
        is.read(reinterpret_cast<char*>(&namesize), sizeof(namesize));
        
        char* boneName = new char[namesize + 1];
        is.read(boneName, namesize);
        boneName[namesize] = '\0';
        uint32_t numOfWeights;
        is.read(reinterpret_cast<char*>(&numOfWeights), sizeof(numOfWeights));
        bone._weights.resize(numOfWeights);

        is.read(reinterpret_cast<char*>(bone._weights.data()), numOfWeights * sizeof(RVertexWeightData));
        bone._boneName = boneName;

        _bones.push_back(bone);
    }
}

std::string RSkeletonData::ToString()
{
    return std::string();
}
