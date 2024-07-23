#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <unordered_map>
#include <string>

struct aiNode;
struct RMeshData;

namespace Faia
{
    namespace BitEngineEditor
    {
        struct BoneNode
        {
            int boneId;
            int parent;
            std::vector<int> childs;
        };

        namespace Importer
        {
            enum ImporterType
            {
                MS,
                ANIM,
                NOT_FOUND
            };

            static const std::unordered_map<std::string, ImporterType> s_typeMap
            {
                {"-ms", MS},
                {"-anim", ANIM}
            };

            class FBXImporter
            {
            public:
                enum State
                {
                    WAITING_START,
                    RUNNING, 
                    DONE, 
                    ERROR
                };

                FBXImporter();
                const char* inputPath;
                const char* outputPath;
                const char* inputRef;

                void Run(const std::string& commandType);
                State GetState();
                std::string GetErrorMsg();

            private:
                std::future<void> _asyncResult;
                std::string mErrorMsg;
                State _state;

                void ImportAnimationAsync();
                void ImportMeshAsync();
            };
        }
    }
}