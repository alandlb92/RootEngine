#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <unordered_map>
#include <string>

namespace Faia
{
    namespace BitEngineEditor
    {
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

                void Run(const std::string& commandType);
                State GetState();
            private:
                std::future<void> _asyncResult;
                void ImportMeshAsync();
                void ImportAnimationAsync();
                State _state;
            };
        }
    }
}