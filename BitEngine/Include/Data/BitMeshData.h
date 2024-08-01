#pragma once
#include <vector>
#include <string>
#include "RMatrix4x4.h"
#include <map>

using namespace std;

#define MAX_NUM_OF_BONES_PER_VERTEX (size_t) 4

namespace Faia
{
    namespace Root
    {
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

        struct RBoneInfo
        {
            std::string mName;
            uint32_t mIndex;
            std::vector<uint32_t> mChildsId;
            /// <summary>
            /// It is sometimes called an inverse-bind matrix, or inverse bind pose matrix.
            /// </summary>
            RMatrix4x4 mBoneOffsetMatrix;
            RMatrix4x4 mNodeTransformationMatrix;
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

        //todo: bonesInfo need to go to a new data specific for bones so we don't need to import for when bones is the same
        struct RBoneInfoData
        {
            RMatrix4x4 mGlovalInverseTransform;
            map<std::string, uint32_t> mBoneNameToIdexMap;
            map<uint32_t, RBoneInfo> mIndexToBoneInfo;

            RBoneInfoData() {}
            RBoneInfoData(const char* filePath) 
            {
                ReadFromPath(filePath);
            }

            void Write(const char* output);
            void ReadFromPath(const char* filePath);
        };

        struct RMeshData
        {
            vector<RMeshNode> _meshs;
            void Write(const char* output);
            void ReadFromPath(const char* filePath);
            std::string ToString();
        };
    }
}