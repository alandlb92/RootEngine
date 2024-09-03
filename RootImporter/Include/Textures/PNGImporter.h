#pragma once
#include <vector>
#include "RImporter.h"

struct RColorRGBA_8b;

namespace Faia
{
    namespace Root
    {
        struct RTextureData;
        namespace Importer 
        {
            class PNGImporter : public RImporter
            {
            public:
                void Run() override;

            private:
                void ConvertToColorArray(const std::vector<unsigned char>& imgBuffer, RTextureData& texData);
            };
        }
    }
}