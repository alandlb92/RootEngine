#pragma once
#include "pch.h"
#include <string>
#include <filesystem>
#include "Data/RMeshData.h"

#include "FBX/FBXMeshImporter.h"
#include "FBX/FBXBoneInfoImporter.h"
#include "FBX/FBXAnimationImporter.h"

const char* cubeFbxRelativePath = "\\TestContent\\ImporterTest\\FBX\\cube.fbx";
const char* heroGoatFbxRelativePath = "\\TestContent\\ImporterTest\\FBX\\HeroGoat.fbx";
const char* heroGoatFbxAnimRelativePath = "\\TestContent\\ImporterTest\\FBX\\HeroGoatTestAnimation.fbx";

const char* cubeRmeshRelativePath = "\\TestContent\\ImporterTest\\RFiles\\cube.rmesh";
const char* heroGoatRboneInfoRelativePath = "\\TestContent\\ImporterTest\\RFiles\\HeroGoat.rboneinfo";
const char* heroGoatRmeshRelativePath = "\\TestContent\\ImporterTest\\RFiles\\HeroGoat.rmesh";
const char* heroGoatRanimRelativePath = "\\TestContent\\ImporterTest\\RFiles\\HeroGoatTestAnimation.ranim";

//todo: now we have Mesh, boneinfo, animation all separeted we need to verify this tests include for textures
namespace ImporterTest
{
    bool IsTheNeededFilesExists()
    {
        std::filesystem::path cubeFBXPath = std::filesystem::current_path();
        cubeFBXPath += cubeFbxRelativePath;
        std::filesystem::path goatFBXPath = std::filesystem::current_path();
        goatFBXPath += heroGoatFbxRelativePath;
        std::filesystem::path goatAnimFBXPath = std::filesystem::current_path();
        goatAnimFBXPath += heroGoatFbxAnimRelativePath;

        bool existsCubeFBX = std::filesystem::exists(cubeFBXPath);
        bool existsGoatFBX = std::filesystem::exists(goatFBXPath);
        bool existsGoatAnimFBX = std::filesystem::exists(goatAnimFBXPath);

        return existsCubeFBX && existsGoatFBX && existsGoatAnimFBX;
    }
}

//Test reach: Read and write .rmeshs (only meshs)
TEST(FBXImporter, ImportJustMesh) 
{
    std::filesystem::path inputPath = std::filesystem::current_path();
    inputPath += cubeFbxRelativePath;
    std::filesystem::path outputPath = std::filesystem::current_path();
    outputPath += cubeRmeshRelativePath;

    Faia::Root::Importer::FBXMeshImporter* meshImporter = new Faia::Root::Importer::FBXMeshImporter();
    
    std::string inputPathStr = inputPath.string();
    std::string  outputPathStr = outputPath.string();

    meshImporter->mArgs.push_back(inputPathStr.c_str());
    meshImporter->mArgs.push_back(outputPathStr.c_str());
    meshImporter->Run();

    Faia::Root::RMeshData rmesh;
    rmesh.ReadFromPath(outputPathStr.c_str());

    delete meshImporter;

    //Verify sizes
    EXPECT_EQ(rmesh.mRMeshNodes.size(), 1);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mIndices.size(), 36);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mVertices.size(), 24);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mNormals.size(), 24);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mUV.size(), 24);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mBoneData.size(), 0);

    //Verify values
    EXPECT_EQ(rmesh.mRMeshNodes[0].mMaterialIndex, 0);

    EXPECT_EQ(rmesh.mRMeshNodes[0].mIndices[0], 0);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mIndices.back(), 23);

    EXPECT_EQ(rmesh.mRMeshNodes[0].mVertices[0], Faia::Root::RVector3D(1.0f));
    EXPECT_EQ(rmesh.mRMeshNodes[0].mVertices.back(), Faia::Root::RVector3D(1.0f, 1.0f, -1.0f));

    EXPECT_EQ(rmesh.mRMeshNodes[0].mNormals[0], Faia::Root::RVector3D(0.0f, 0.0f, 1.0f));
    EXPECT_EQ(rmesh.mRMeshNodes[0].mNormals.back(), Faia::Root::RVector3D(0.0f, 1.0f, 0.0f));

    EXPECT_EQ(rmesh.mRMeshNodes[0].mUV[0], Faia::Root::RVector2D(0.625f, -0.5f));
    EXPECT_EQ(rmesh.mRMeshNodes[0].mUV.back(), Faia::Root::RVector2D(0.375f, -0.5f));
}


//Test reach: Read and write .rboneinfo
TEST(FBXImporter, ImportBoneinfo)
{
    std::filesystem::path inputPath = std::filesystem::current_path();
    inputPath += heroGoatFbxRelativePath;
    std::filesystem::path outputPath = std::filesystem::current_path();
    outputPath += heroGoatRboneInfoRelativePath;

    Faia::Root::Importer::FBXBoneInfoImporter* boneInfoImporter = new Faia::Root::Importer::FBXBoneInfoImporter();

    std::string inputPathStr = inputPath.string();
    std::string  outputPathStr = outputPath.string();

    boneInfoImporter->mArgs.push_back(inputPathStr.c_str());
    boneInfoImporter->mArgs.push_back(outputPathStr.c_str());
    boneInfoImporter->Run();

    Faia::Root::RBoneInfoData rBoneInfo;
    rBoneInfo.ReadFromPath(outputPathStr.c_str());

    delete boneInfoImporter;

    //Verify sizes
    EXPECT_EQ(rBoneInfo.mBoneNameToIdexMap.size(), 53);
    EXPECT_EQ(rBoneInfo.mIndexToBoneInfo.size(), 53);

    //Verify values
    Faia::Root::RMatrix4x4 expectedInverseTransform(
        1.0f, .0f, .0f, .0f,
        .0f, 1.0f, .0f, .0f,
        .0f, .0f, 1.0f, .0f,
        .0f, .0f, .0f, 1.0f
    );

    EXPECT_EQ(rBoneInfo.mGlobalInverseTransform, expectedInverseTransform);
    EXPECT_EQ(rBoneInfo.mBoneNameToIdexMap["mixamorig:Head"], 5);
    EXPECT_EQ(rBoneInfo.mBoneNameToIdexMap["mixamorig:Spine2"], 3);

    Faia::Root::RBoneInfo firstBoneInfo = rBoneInfo.mIndexToBoneInfo[0];
    Faia::Root::RBoneInfo lastBoneInfo = (std::prev(rBoneInfo.mIndexToBoneInfo.end()))->second;

    EXPECT_TRUE(std::strcmp(firstBoneInfo.mName.c_str(), "mixamorig:Hips") == 0);
    EXPECT_EQ(firstBoneInfo.mIndex, 0);
    EXPECT_EQ(firstBoneInfo.mChildsId.size(), 3);
    EXPECT_EQ(firstBoneInfo.mChildsId[0], 1);
    EXPECT_EQ(firstBoneInfo.mChildsId.back(), 45);
    EXPECT_EQ(firstBoneInfo.mBoneOffsetMatrix.m03, -0.140888214f);
    EXPECT_EQ(firstBoneInfo.mBoneOffsetMatrix.m23, -1.82585418f);
    EXPECT_EQ(firstBoneInfo.mNodeTransformationMatrix.m03, 0.140888214f);
    EXPECT_EQ(firstBoneInfo.mNodeTransformationMatrix.m23, 1.82585418f);


    EXPECT_TRUE(std::strcmp(lastBoneInfo.mName.c_str(), "mixamorig:HeadTop_End") == 0);
    EXPECT_EQ(lastBoneInfo.mIndex, 52);
    EXPECT_EQ(lastBoneInfo.mChildsId.size(), 0);
    EXPECT_EQ(lastBoneInfo.mBoneOffsetMatrix.m03, -0.140888214f);
    EXPECT_EQ(lastBoneInfo.mBoneOffsetMatrix.m23, -5.85805511f);
    EXPECT_EQ(lastBoneInfo.mNodeTransformationMatrix.m03, .0f);
    EXPECT_EQ(lastBoneInfo.mNodeTransformationMatrix.m23, 6.96930456f);
}

//Test reach: Read and write .rmeshs (with bone info)
TEST(FBXImporter, ImportMeshWithBoneInfo)
{
    std::filesystem::path inputPath = std::filesystem::current_path();
    inputPath += heroGoatFbxRelativePath;
    std::filesystem::path outputPath = std::filesystem::current_path();
    outputPath += heroGoatRmeshRelativePath;
    std::filesystem::path refPath = std::filesystem::current_path();
    refPath += heroGoatRboneInfoRelativePath;

    Faia::Root::Importer::FBXMeshImporter* meshImporter = new Faia::Root::Importer::FBXMeshImporter();

    std::string inputPathStr = inputPath.string();
    std::string  outputPathStr = outputPath.string();
    std::string  refPathStr = refPath.string();

    meshImporter->mArgs.push_back(inputPathStr.c_str());
    meshImporter->mArgs.push_back(outputPathStr.c_str());
    meshImporter->mArgs.push_back(refPathStr.c_str());
    meshImporter->Run();

    Faia::Root::RMeshData rmesh;
    rmesh.ReadFromPath(outputPathStr.c_str());

    delete meshImporter;

    //Verify sizes
    EXPECT_EQ(rmesh.mRMeshNodes.size(), 5);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mIndices.size(), 90093);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mVertices.size(), 17000);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mNormals.size(), 17000);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mUV.size(), 17000);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mBoneData.size(), 17000);

    EXPECT_EQ(rmesh.mRMeshNodes.back().mIndices.size(), 2448);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mVertices.size(), 861);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mNormals.size(), 861);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mUV.size(), 861);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mBoneData.size(), 861);

    //Verify values
    EXPECT_EQ(rmesh.mRMeshNodes[0].mMaterialIndex, 0);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mMaterialIndex, 3);

    EXPECT_EQ(rmesh.mRMeshNodes[0].mIndices[0], 4);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mIndices.back(), 15795);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mIndices[0], 0);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mIndices.back(), 860);

    EXPECT_EQ(rmesh.mRMeshNodes[0].mVertices[0], Faia::Root::RVector3D(11.5714121f, 126.246719f, 4.65061283f));
    EXPECT_EQ(rmesh.mRMeshNodes[0].mVertices.back(), Faia::Root::RVector3D(-9.68652344f, 120.549553f, 4.89830399f));
    EXPECT_EQ(rmesh.mRMeshNodes.back().mVertices[0], Faia::Root::RVector3D(-0.700381994f, 159.894638f, 10.6821823f));
    EXPECT_EQ(rmesh.mRMeshNodes.back().mVertices.back(), Faia::Root::RVector3D(-0.586758971f, 165.443375f, 11.3377914f));

    EXPECT_EQ(rmesh.mRMeshNodes[0].mNormals[0], Faia::Root::RVector3D(0.956207216f, -0.292055070f, 0.0192740038f));
    EXPECT_EQ(rmesh.mRMeshNodes[0].mNormals.back(), Faia::Root::RVector3D(-0.802569628f, 0.501011789f, 0.323834836f));
    EXPECT_EQ(rmesh.mRMeshNodes.back().mNormals[0], Faia::Root::RVector3D(-0.701394320f, 0.0542889796f, 0.710703194f));
    EXPECT_EQ(rmesh.mRMeshNodes.back().mNormals.back(), Faia::Root::RVector3D(-0.276736021f, -0.0109040011f, 0.960884094f));

    EXPECT_EQ(rmesh.mRMeshNodes[0].mUV[0], Faia::Root::RVector2D(0.421817005f, -0.931591988f));
    EXPECT_EQ(rmesh.mRMeshNodes[0].mUV.back(), Faia::Root::RVector2D(0.421011001f, -0.236858994f));
    EXPECT_EQ(rmesh.mRMeshNodes.back().mUV[0], Faia::Root::RVector2D(0.926757991f, -0.745123982f));
    EXPECT_EQ(rmesh.mRMeshNodes.back().mUV.back(), Faia::Root::RVector2D(0.772275984f, -0.648880005f));

    EXPECT_EQ(rmesh.mRMeshNodes[0].mBoneData[0].boneId[2], 3);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mBoneData[0].weights[2], 0.00622486323f);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mBoneData.back().boneId[2], 2);
    EXPECT_EQ(rmesh.mRMeshNodes[0].mBoneData.back().weights[2], 0.0970864668f);

    EXPECT_EQ(rmesh.mRMeshNodes.back().mBoneData[0].boneId[2], 0);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mBoneData[0].weights[2], .0f);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mBoneData.back().boneId[2], 0);
    EXPECT_EQ(rmesh.mRMeshNodes.back().mBoneData.back().weights[2], .0f);
}


//Test reach: Read and write .ranim
TEST(FBXImporter, ImportAnimation)
{
    std::filesystem::path inputPath = std::filesystem::current_path();
    inputPath += heroGoatFbxAnimRelativePath;
    std::filesystem::path outputPath = std::filesystem::current_path();
    outputPath += heroGoatRanimRelativePath;
    std::filesystem::path refPath = std::filesystem::current_path();
    refPath += heroGoatRboneInfoRelativePath;

    Faia::Root::Importer::FBXAnimationImporter* animImporter = new Faia::Root::Importer::FBXAnimationImporter();

    std::string inputPathStr = inputPath.string();
    std::string  outputPathStr = outputPath.string();
    std::string  refPathStr = refPath.string();

    animImporter->mArgs.push_back(inputPathStr.c_str());
    animImporter->mArgs.push_back(outputPathStr.c_str());
    animImporter->mArgs.push_back(refPathStr.c_str());
    animImporter->Run();

    Faia::Root::RAnimationData ranimData;
    ranimData.ReadFromPath(outputPathStr.c_str());

    delete animImporter;

    EXPECT_TRUE(std::strcmp(ranimData.mName.c_str(), "mixamo.com") == 0);
    EXPECT_EQ(ranimData.mDuration, 115.0f);
    EXPECT_EQ(ranimData.mTicksPerSecond, 30.0f);
    EXPECT_EQ(ranimData.mAnimChannels.size(), 52);

    EXPECT_EQ(ranimData.mAnimChannels[0].mBoneId, 0);
    EXPECT_EQ(ranimData.mAnimChannels[0].mPositions[9].mTime, 9.f);
    EXPECT_EQ(ranimData.mAnimChannels[0].mPositions[9].mValue, Faia::Root::RVector3D(9.71774292f, 104.502266f, 6.37402725f));
    EXPECT_EQ(ranimData.mAnimChannels[0].mScales[9].mTime, 9.f);
    EXPECT_EQ(ranimData.mAnimChannels[0].mScales[9].mValue, Faia::Root::RVector3D(1.f, 1.f, 0.999999940f));
    EXPECT_EQ(ranimData.mAnimChannels[0].mRotations[9].mTime, 9.f);
    EXPECT_EQ(ranimData.mAnimChannels[0].mRotations[9].mValue, Faia::Root::RQuaternion(-0.0804047585f, -0.122044772f, -0.0798984095f, 0.986030638f));
}