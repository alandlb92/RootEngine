#include "FBXImporter.h"
#include "Data/BitMeshData.h"
#include <iostream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>

#include "FBXMeshImporter.h"
#include "FBXBoneInfoImporter.h"
#include "FBXAnimationImporter.h"

namespace Faia
{
    namespace BitEngineEditor
    {
        namespace Importer
        {
            FBXImporter::FBXImporter()
            {
                mState = WAITING_START;
            }            

            ImporterState FBXImporter::GetState()
            {
                return mState;
            }

            std::string FBXImporter::GetErrorMsg()
            {
                return mErrorMsg;
            }

            RMatrix4x4 FBXImporter::AiMatrixToRMatrix(aiMatrix4x4 assimpMatrix)
            {
                RMatrix4x4 result;

                result.m00 = assimpMatrix.a1; result.m01 = assimpMatrix.a2; result.m02 = assimpMatrix.a3; result.m03 = assimpMatrix.a4;
                result.m10 = assimpMatrix.b1; result.m11 = assimpMatrix.b2; result.m12 = assimpMatrix.b3; result.m13 = assimpMatrix.b4;
                result.m20 = assimpMatrix.c1; result.m21 = assimpMatrix.c2; result.m22 = assimpMatrix.c3; result.m23 = assimpMatrix.c4;
                result.m30 = assimpMatrix.d1; result.m31 = assimpMatrix.d2; result.m32 = assimpMatrix.d3; result.m33 = assimpMatrix.d4;

                return result;
            }

            std::unique_ptr<FBXImporter> FBXImporter::GetImporter(int argc, char* argv[])
            {
                std::unique_ptr<FBXImporter> importer;
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

            ImporterType FBXImporter::GetImporterType(char* importerName)
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