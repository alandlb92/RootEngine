#include "RImporter.h"
#include "Data/RMeshData.h"
#include <iostream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>

#include "FBX/FBXMeshImporter.h"
#include "FBX/FBXBoneInfoImporter.h"
#include "FBX/FBXAnimationImporter.h"
#include "FBX/FBXMeshToJsonImporter.h"

#include "Textures/JPGImporter.h"
#include "Textures/PNGImporter.h"

namespace Faia
{
    namespace Root
    {
        namespace Importer
        {
            RImporter::RImporter()
            {
                mState = WAITING_START;
            }            

            ImporterState RImporter::GetState()
            {
                return mState;
            }

            std::string RImporter::GetErrorMsg()
            {
                return mErrorMsg;
            }

            RMatrix4x4 RImporter::AiMatrixToRMatrix(aiMatrix4x4 assimpMatrix)
            {
                RMatrix4x4 result;

                result.m00 = assimpMatrix.a1; result.m01 = assimpMatrix.a2; result.m02 = assimpMatrix.a3; result.m03 = assimpMatrix.a4;
                result.m10 = assimpMatrix.b1; result.m11 = assimpMatrix.b2; result.m12 = assimpMatrix.b3; result.m13 = assimpMatrix.b4;
                result.m20 = assimpMatrix.c1; result.m21 = assimpMatrix.c2; result.m22 = assimpMatrix.c3; result.m23 = assimpMatrix.c4;
                result.m30 = assimpMatrix.d1; result.m31 = assimpMatrix.d2; result.m32 = assimpMatrix.d3; result.m33 = assimpMatrix.d4;

                return result;
            }

            std::unique_ptr<RImporter> RImporter::GetImporter(int argc, char* argv[])
            {
                std::unique_ptr<RImporter> importer;
                switch (GetImporterType(argv[2]))
                {
                case MS:
                    importer = std::make_unique<FBXMeshImporter>();
                    break;
                case ANIM:
                    importer = std::make_unique<FBXAnimationImporter>();
                    break;
                case BONE:
                    importer = std::make_unique<FBXBoneInfoImporter>();
                    break;
                case TEXTURE_JPG:
                    importer = std::make_unique<JPGImporter>();
                    break;
                case TEXTURE_PNG:
                    importer = std::make_unique<PNGImporter>();
                    break;
                case MS_JSON:
                    importer = std::make_unique<FBXMeshToJsonImporter>();
                    break;
                case NOT_FOUND:
                    return nullptr;
                    break;
                }

                for (int i = 3; i < argc; ++i)
                {
                    importer->mArgs.push_back(argv[i]);
                }

                return importer;
            }

            ImporterType RImporter::GetImporterType(char* importerName)
            {
                std::string name(importerName);
                ImporterType importerType = NOT_FOUND;
                std::unordered_map<std::string, ImporterType>::const_iterator it = sImporterNameToType.find(name);
                if (it != sImporterNameToType.end())
                {
                    importerType = it->second;
                }

                return importerType;
            }

        }
    }
}