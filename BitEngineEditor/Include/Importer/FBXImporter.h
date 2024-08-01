#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <unordered_map>
#include <string>
#include "assimp/matrix4x4.h"
#include <memory>

struct aiNode;
struct RMeshData;
struct RAnimationData;
struct RMatrix4x4;

namespace Faia
{
    namespace BitEngineEditor
    {
        namespace Importer
        {
            //todo: specific of bone
            struct BoneNode
            {
                int boneId;
                int parent;
                std::vector<int> childs;
            };

            enum ImporterType
            {
                MS,
                ANIM,
                BONE,
                NOT_FOUND
            };

            static const std::unordered_map<std::string, ImporterType> sImporterNameToType
            {
                {"-ms", MS},
                {"-anim", ANIM},
                {"-bone", BONE}
            };

            enum ImporterState
            {
                WAITING_START,
                RUNNING,
                DONE,
                ERROR
            };

            class FBXImporter
            {
            public:
                static std::unique_ptr<FBXImporter> GetImporter(int argc, char* argv[]);

                FBXImporter();

                virtual void Run() = 0;

                ImporterState GetState();
                std::string GetErrorMsg();

                std::vector<const char*> mArgs;

            protected:
                RMatrix4x4 AiMatrixToRMatrix(aiMatrix4x4 assimpMatrix);

                std::future<void> mAsyncResult;
                std::string mErrorMsg;
                ImporterState mState;

            private:
                static ImporterType GetImporterType(char* importerName);
            };
        }
    }
}