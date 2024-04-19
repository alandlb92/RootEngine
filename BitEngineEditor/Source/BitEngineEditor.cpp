// BitEngineEditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Importer/FBXImporter.h"

int main(int argc, char* argv[])
{
    std::cout << "Start Root Engine Editor\n";

    Faia::BitEngineEditor::Importer::FBXImporter fbxImporter;
    
    if (argc < 3) {
        std::cerr << "\033[1;31mError:\033[0m Input and output paths are required." << std::endl;
        return 1;
    }

    fbxImporter.inputPath = argv[1];
    fbxImporter.outputPath = argv[2];
    
    fbxImporter.Run();
    std::cout << "Start import!\n";    
    while (fbxImporter.GetState() == Faia::BitEngineEditor::Importer::FBXImporter::WAITING_START
        || fbxImporter.GetState() == Faia::BitEngineEditor::Importer::FBXImporter::RUNNING)
    {
        std::cout << "\\" << "\r";
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        std::cout << "|" << "\r";
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        std::cout << "/" << "\r";
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        std::cout << "-" << "\r";
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    if (fbxImporter.GetState() == Faia::BitEngineEditor::Importer::FBXImporter::ERROR)
        std::cout << "Import has encountered an \033[1;31mERROR!\033[0m when try to import: " << fbxImporter.inputPath << "\n";
    else
        std::cout << "Import is DONE! result: " << fbxImporter.outputPath << "\n";
}