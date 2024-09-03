#include "RImporter.h"
#include <iostream>
#include <string>

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
        
    if (std::strcmp(argv[1], "import") != 0)
    {
        std::cerr << "\033[1;31mError:\033[0m" << argv[1] << " Not recognized" << "\n";
        return 1;
    }

    std::unique_ptr<Faia::Root::Importer::RImporter> importer = Faia::Root::Importer::RImporter::GetImporter(argc, argv);

    if (!importer)
    {
        std::cerr << "\033[1;31mError:\033[0m The argument " << argv[2] << " was not recognized\n";
        std::cout << "Thes list of arguments are:\n";
        
        for (auto arg : Faia::Root::Importer::sImporterNameToType)
        {
            std::cout << "    " << arg.first << "\n";
        }
        
        return 1;
    }

    std::cout << "Start command! " << argv[2] << "\n";

    importer->Run();

    while (importer->GetState() == Faia::Root::Importer::WAITING_START
        || importer->GetState() == Faia::Root::Importer::RUNNING)
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

    if (importer->GetState() == Faia::Root::Importer::ERROR)
    {
        std::cout << "Import has found an \033[1;31mERROR!" << "\n-> " << importer->GetErrorMsg() << "\n";
    }
    else
    {
        std::cout << "Import is DONE!\n";
    }
}