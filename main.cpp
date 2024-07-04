#include <fbxsdk.h>
#include <string>
#include <iostream>
#include "../Common/Common.h" 

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: provide 1 argument with path to source .fbx file for conversion into .m2 format.\n";
    }
	std::string sourceFbx = argv[1];
    int dotPos = sourceFbx.find('.');
    std::string outputM2 = sourceFbx.substr(0, dotPos) + ".m2";

    FbxManager* manager = nullptr;
    FbxScene* scene = nullptr;

    InitializeSdkObjects(manager, scene);

	bool isCompleted = LoadScene(manager, scene, sourceFbx.c_str());
    if(!isCompleted) {
        std::cout << "Call to LoadScene() failed.\n";
        return -1;
    }

    int writerId = manager->GetIOPluginRegistry()->FindWriterIDByExtension("m2");
    if (writerId < 0) {
        std::cout << "The writer for extension m2 was not found";
        return;
    }
    std::cout << "The writer id is " << writerId << std::endl;

    FbxExporter* exporter = FbxExporter::Create(manager, "");

    isCompleted = exporter->Initialize(outputM2.c_str(), writerId, manager->GetIOSettings());
    if(!isCompleted)
    {
        std::cout << "%s:\tCall to FbxExporter::Initialize() failed.\n";
        std::cout << "Error returned:" << exporter->GetStatus().GetErrorString() << "\n";
        return -1;
    }

    isCompleted = exporter->Export(scene);
    if(!isCompleted)
    {
        std::cout << "Call to FbxExporter::Export() failed.\n";
    }

    exporter->Destroy();

    DestroySdkObjects(manager, isCompleted);
    return isCompleted;
}

