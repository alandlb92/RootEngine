#include "FBX/FBXMeshToJsonImporter.h"
#include "FBX/FBXMeshImporter.h"
#include "nlohmann/json.hpp"
#include "Data/RMeshData.h"
#include <fstream>;

void Faia::Root::Importer::FBXMeshToJsonImporter::Run()
{
    FBXMeshImporter meshImporter;
    meshImporter.mArgs = mArgs;
    meshImporter.Run();

    if (meshImporter.GetState() != DONE)
    {
        mState = meshImporter.GetState();
        return;
    }

    RMeshData data;
    data.ReadFromPath(mArgs[1]);

    nlohmann::json json;
    for (int i = 0;i < data.mRMeshNodes.size(); i++)
    {
        std::string nodeName = "node_" + std::to_string(i);
        std::vector<float> vertices = std::vector<float>(data.mRMeshNodes[i].mVertices.size() * 3);
        memcpy(vertices.data(), data.mRMeshNodes[i].mVertices.data(), data.mRMeshNodes[i].mVertices.size() * 3 * sizeof(float));
        json[nodeName]["vertices"] = vertices;
        json[nodeName]["indices"] = data.mRMeshNodes[i].mIndices;
    }
    std::string jsonOutPath = mArgs[1] + std::string(".json");
    std::ofstream o(jsonOutPath);
    o << std::setw(4) << json << std::endl;

    mState = DONE;
}
