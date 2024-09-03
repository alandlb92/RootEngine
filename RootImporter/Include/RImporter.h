#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <unordered_map>
#include <string>
#include <memory>

#include "assimp/matrix4x4.h"

struct aiNode;
struct RMeshData;

namespace Faia
{
    namespace Root
    {
        struct RAnimationData;
        struct RMatrix4x4;

        namespace Importer
        {

            enum ImporterType
            {
                MS,
                ANIM,
                BONE,
                NOT_FOUND,
                TEXTURE_JPG,
                TEXTURE_PNG
            };

            static const std::unordered_map<std::string, ImporterType> sImporterNameToType
            {
                {"-ms", MS},
                {"-anim", ANIM},
                {"-bone", BONE},
                {"-texjpg", TEXTURE_JPG},
                {"-texpng", TEXTURE_PNG}
            };

            enum ImporterState
            {
                WAITING_START,
                RUNNING,
                DONE,
                ERROR
            };

            class RImporter
            {
            public:
                static std::unique_ptr<RImporter> GetImporter(int argc, char* argv[]);

                RImporter();

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