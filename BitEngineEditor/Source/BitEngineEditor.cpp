// BitEngineEditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Importer/FBXImporter.h"


// params:
// arg[0] is the .exe 
// arg[1] can be:
//      "import"
// arg[2] can be:
//      "-ms" mesh and skeleton 
//      "-anim" animation
//arg[3] input file
//arg[4] output file

int main(int argc, char* argv[])
{
    std::cout << "Start Root Engine Editor\n";

    if (argc < 3) {
        std::cerr << "\033[1;31mError:\033[0m Input and output paths are required.\n";
        return 1;
    }

    std::unique_ptr<Faia::BitEngineEditor::Importer::FBXImporter> fbxImporter = Faia::BitEngineEditor::Importer::FBXImporter::GetImporter(argc, argv);
    if (!fbxImporter)
    {
        std::cerr << "\033[1;31mError:\033[0m The argument " << argv[2] << " was not recognized\n";
        std::cout << "Thes list of arguments are:\n";
        
        for (auto arg : Faia::BitEngineEditor::Importer::sImporterNameToType)
        {
            std::cout << "    " << arg.first << "\n";
        }
        
        return 1;
    }

    std::cout << "Start command! " << argv[2] << "\n";


    fbxImporter->Run();
   
    
    while (fbxImporter->GetState() == Faia::BitEngineEditor::Importer::WAITING_START
        || fbxImporter->GetState() == Faia::BitEngineEditor::Importer::RUNNING)
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

    if (fbxImporter->GetState() == Faia::BitEngineEditor::Importer::ERROR)
    {
        std::cout << "Import has found an \033[1;31mERROR!" << "\n-> " << fbxImporter->GetErrorMsg() << "\n";
    }
    else
    {
        std::cout << "Import is DONE!\n";
    }
}