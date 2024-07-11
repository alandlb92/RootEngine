#pragma once
#include <vector>
#include <string>
#include "Quaternion.h"
#include <map>

using namespace std;

#define MAX_NUM_OF_BONES_PER_VERTEX (size_t) 4

struct RVertexBoneData
{
    uint32_t boneId[MAX_NUM_OF_BONES_PER_VERTEX] = {0};
    float weights[MAX_NUM_OF_BONES_PER_VERTEX] = {0.0f};
};

struct RVertexWeightData
{
    int vertexId;
    float weight;
};

struct RBone
{
    const char* _boneName; 
    vector<RVertexWeightData> _weights;
};

struct RSkeletonData
{
    vector<RBone> _bones;
    void Write(std::ofstream& os);
    void Read(std::ifstream& is);
    std::string ToString();
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
    Vector3D mValue;
};

struct RAnimationVectorKey
{
    float mTime;
    Quaternion mValue;
};

struct RAnimationChannel
{
   // RAnimationVectorKey m
};

struct RAnimationData
{
    std::string mName;
    float mDuration;
    float mTicksPerSecond;
    //std::vector
};

struct RMeshData
{
    vector<RMeshNode> _meshs;
    map<std::string, uint32_t> _boneNameToIdexMap;
    void Write(const char* output);
    void ReadFromPath(const char* filePath);
    std::string ToString();
    RAnimationData mAnimation;
};