#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

namespace Faia
{
    namespace BitEngineEditor
    {
        namespace Importer
        {
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

                void Run();
                State GetState();
            private:
                std::future<void> _asyncResult;
                void ImportAsync();
                State _state;
            };
        }
    }
}