#pragma once
#include <vector>
#include <string>
#include "Vector3D.h"

using namespace std;

struct BitMeshNode
{
    vector<uint16_t> _indices;
    vector<Vector3D> _vertices;
    vector<Vector2D> _uv;
    uint16_t _materialIndex;
};

struct BitMeshData
{
    vector<BitMeshNode> _meshs;
    void Write(const char* output);
    void ReadFromPath(const char* filePath);
    std::string ToString();
    
};