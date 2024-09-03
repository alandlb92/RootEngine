#include "Textures/JPGImporter.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Data/RMeshData.h"
#include "Data/RColor.h"

#include <turbojpeg.h>

namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            std::vector<RColorRGBA_8b> JPGImporter::ConvertToColorArray(const std::vector<unsigned char>& imgBuffer, int width, int height)
            {
                const size_t numPixels = width * height;
                const size_t bytesPerPixel = tjPixelSize[TJPF_RGBA]; // RGB
                
                std::vector<RColorRGBA_8b> colorArray(numPixels);
                std::memcpy(colorArray.data(), imgBuffer.data(), numPixels* bytesPerPixel);

                return colorArray;
            }


            void JPGImporter::Run()
            {
                const char* inputPath = mArgs[0];
                const char* outputPath = mArgs[1];

                std::ifstream file(inputPath, std::ios::binary);
                std::vector<unsigned char> jpgBuffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                RTextureData texData;

                tjhandle tjInstance = tjInitDecompress();
                if (!tjInstance) {
                    std::stringstream ss;
                    ss << "TurboJPEG can't start: " << tjGetErrorStr();
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                int width, height, jpegSubsamp, jpegColorspace;
                if (tjDecompressHeader3(tjInstance, jpgBuffer.data(), jpgBuffer.size(), &width, &height, &jpegSubsamp, &jpegColorspace) < 0) 
                {
                    std::stringstream ss;
                    ss << "Can't read JPG header: " << tjGetErrorStr();
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                std::vector<unsigned char> imgBuffer(width * height * tjPixelSize[TJPF_RGBA]);
                if (tjDecompress2(tjInstance, jpgBuffer.data(), jpgBuffer.size(), imgBuffer.data(), width, width * tjPixelSize[TJPF_RGBA], height, TJPF_RGBA, 0) < 0)
                {
                    std::stringstream ss;
                    ss << "Can't decompress JPG: " << tjGetErrorStr();
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                tjDestroy(tjInstance);
                
                texData.mHeight = height;
                texData.mWidth = width;
                texData.mPixels = ConvertToColorArray(imgBuffer, width, height);
                texData.Write(outputPath);

                mState = DONE;
            }
        }
    }
}