#include "Data/BitMeshData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

void BitMeshData::Write(const char* output)
{
    std::ofstream os(output, std::ios::binary);
    if (!os) {
        stringstream ss;
        ss << "of stream cant open outPut path: " << output << std::endl;
        throw std::invalid_argument(ss.str().c_str());
    }

    uint32_t numMeshs = _meshs.size();
    os.write(reinterpret_cast<char*>(&numMeshs), sizeof(numMeshs));
    for (const auto& node : _meshs)
    {
        uint32_t numIndices = node._indices.size();
        uint32_t numVertices = node._vertices.size();
        uint32_t numUV = node._uv.size();
        uint32_t numNormals = node._uv.size();

        os.write(reinterpret_cast<char*>(&numIndices), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numVertices), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numNormals), sizeof(uint32_t));
        os.write(reinterpret_cast<char*>(&numUV), sizeof(uint32_t));

        os.write(reinterpret_cast<const char*>(node._indices.data()), numIndices * sizeof(uint16_t));
        os.write(reinterpret_cast<const char*>(node._vertices.data()), numVertices * sizeof(Vector3D));
        os.write(reinterpret_cast<const char*>(node._normals.data()), numNormals * sizeof(Vector3D));
        os.write(reinterpret_cast<const char*>(node._uv.data()), numUV * sizeof(Vector2D));
        os.write(reinterpret_cast<const char*>(&node._materialIndex), sizeof(uint16_t));

    }

    os.close();
}

void BitMeshData::ReadFromPath(const char* filePath)
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
        BitMeshNode node;
        uint32_t numIndices, numVertices, numUV, numNormals;
        is.read(reinterpret_cast<char*>(&numIndices), sizeof(uint32_t));
        is.read(reinterpret_cast<char*>(&numVertices), sizeof(uint32_t));
        is.read(reinterpret_cast<char*>(&numNormals), sizeof(uint32_t));
        is.read(reinterpret_cast<char*>(&numUV), sizeof(uint32_t));

        node._indices.resize(numIndices);
        node._vertices.resize(numVertices);
        node._uv.resize(numUV);
        node._normals.resize(numNormals);

        is.read(reinterpret_cast<char*>(node._indices.data()), numIndices * sizeof(uint16_t));
        is.read(reinterpret_cast<char*>(node._vertices.data()), numVertices * sizeof(Vector3D));
        is.read(reinterpret_cast<char*>(node._normals.data()), numVertices * sizeof(Vector3D));
        is.read(reinterpret_cast<char*>(node._uv.data()), numUV * sizeof(Vector2D));
        is.read(reinterpret_cast<char*>(&node._materialIndex), sizeof(uint16_t));

        _meshs.push_back(node);
    }

    is.close();
}

std::string BitMeshData::ToString()
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