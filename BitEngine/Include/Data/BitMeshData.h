#pragma once
#include <vector>
#include <string>
#include "Vector3D.h"

using namespace std;

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
    uint16_t _materialIndex;
    RSkeletonData _skeletonData;
};

struct RMeshData
{
    vector<RMeshNode> _meshs;
    void Write(const char* output);
    void ReadFromPath(const char* filePath);
    std::string ToString();
};
