#pragma once
#include <vector>
#include <string>
#include "Quaternion.h"
#include <map>

using namespace std;

#define MAX_NUM_OF_BONES_PER_VERTEX (size_t) 4

struct RVertexBoneData
{
    uint32_t boneId[MAX_NUM_OF_BONES_PER_VERTEX] = { 0 };
    float weights[MAX_NUM_OF_BONES_PER_VERTEX] = { 0.0f };
};

struct RVertexWeightData
{
    int vertexId;
    float weight;
};

struct RMeshNode
{
    vector<uint16_t> _indices;
    vector<Vector3D> _vertices;
    vector<Vector3D> _normals;
    vector<Vector2D> _uv;
    vector<RVertexBoneData> _boneData;
    uint16_t _materialIndex;
};

struct RAnimationQuatKey
{
    float mTime;
    Quaternion mValue;
};

struct RAnimationVectorKey
{
    float mTime;
    Vector3D mValue;
};

struct RAnimationChannel
{
    uint32_t mBoneId;
    std::vector<RAnimationVectorKey> mPositions;
    std::vector<RAnimationVectorKey> mScales;
    std::vector<RAnimationQuatKey> mRotations;
};

struct RAnimationData
{
    std::string mName;
    float mDuration;
    float mTicksPerSecond;
    std::vector<RAnimationChannel> mAnimChannels;
    void Write(const char* output);
    void ReadFromPath(const char* filePath);
};

struct RBoneInfo
{
    std::string mName;
    uint32_t mIndex;
    int32_t mParentIndex;
};

struct RMeshData
{
    vector<RMeshNode> _meshs;
    map<std::string, uint32_t> _boneNameToIdexMap;
    map<uint32_t, RBoneInfo> mIndexToBoneInfo;
    void Write(const char* output);
    void ReadFromPath(const char* filePath);
    std::string ToString();
};