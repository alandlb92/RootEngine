#include "Textures/PNGImporter.h"

#include <stdio.h>

#include <png.h>

#include "Data/RMeshData.h"
#include "Data/RColor.h"



namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            void PNGImporter::ConvertToColorArray(const std::vector<unsigned char>& imgBuffer, RTextureData& texData)
            {                
                const size_t numPixels = texData.mWidth * texData.mHeight;
                const size_t bytesPerPixel = 4; // RGBA
                
                texData.mPixels = std::vector<RColorRGBA_8b>(numPixels);
                std::memcpy(texData.mPixels.data(), imgBuffer.data(), numPixels * bytesPerPixel);
            }


            void PNGImporter::Run()
            {
                const char* inputPath = mArgs[0];
                const char* outputPath = mArgs[1];
                RTextureData texData;
                FILE* fp;
                fopen_s(&fp, inputPath, "rb");

                if (!fp)
                {
                    std::stringstream ss;
                    ss << "can't open: " << inputPath;
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                if (!png) {
                    std::stringstream ss;
                    ss << "can't create png struct";
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                png_infop info = png_create_info_struct(png);
                if (!info) {
                    std::stringstream ss;
                    ss << "can't create png info";
                    mErrorMsg = ss.str();
                    mState = ERROR;
                    return;
                }

                png_init_io(png, fp);
                png_read_info(png, info);

                png_byte color_type = png_get_color_type(png, info);
                png_byte bit_depth = png_get_bit_depth(png, info);
                if (color_type == PNG_COLOR_TYPE_RGB) {
                    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
                }

                if (bit_depth == 16) {
                    png_set_strip_16(png);
                }
                
                texData.mWidth = png_get_image_width(png, info);
                texData.mHeight = png_get_image_height(png, info);

                png_set_strip_alpha(png);
                png_set_expand(png);
                png_set_gray_to_rgb(png);

                std::vector<unsigned char> imgBuffer;
                imgBuffer.resize(texData.mWidth * texData.mHeight * 4); // 4 bytes por pixel (RGBA)

                std::vector<png_bytep> row_pointers = std::vector<png_bytep>(texData.mHeight);

                for (int y = 0; y < texData.mHeight; y++) {
                    row_pointers[y] = imgBuffer.data() + y * texData.mWidth * 4;
                }


                png_read_image(png, row_pointers.data());

                png_destroy_read_struct(&png, &info, nullptr);
                ConvertToColorArray(imgBuffer, texData);

                fclose(fp);
                texData.Write(outputPath);

                mState = DONE;
            }
        }
    }
}