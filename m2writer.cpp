#include "m2writer.h"

// Create your own writer.
// And your writer will get a pPluginID and pSubID. 
FbxWriter* CreateM2Writer(FbxManager& manager, FbxExporter& /*exporter*/, int /*subId*/, int pluginId)
{
    // use FbxNew instead of new, since FBX will take charge its deletion
    return FbxNew<M2Writer>(manager, pluginId);
}

// Get extension, description or version info about MyOwnWriter
void* GetM2WriterInfo(FbxWriter::EInfoRequest request, int /*Id*/)
{
    static const char* ext[] = 
    {
        "m2",
        0
    };

    static const char* desc[] = 
    {
        "https://wowdev.wiki/M2",
        0
    };

    switch (request)
    {
    case FbxWriter::eInfoExtension:
        return ext;
    case FbxWriter::eInfoDescriptions:
        return desc;
    case FbxWriter::eInfoVersions: // do not know if we need this one for m2writer
        return 0;
    default:
        return 0;
    }
}

void FillM2WriterIOSettings(FbxIOSettings& /*pIOS*/)
{
    // do not know if we need this one for m2writer
}

M2Writer::M2Writer(FbxManager &manager, int id) :
    FbxWriter(manager, id, FbxStatusGlobal::GetRef()), _file(nullptr), _manager(&manager)
{

}

M2Writer::~M2Writer()
{
    FileClose();
}

// Create a file stream with pFileName
bool M2Writer::FileCreate(char* fileName)
{
    if(_file != nullptr)
    {
        FileClose();
    }
    FBXSDK_fopen(_file, fileName, "w");
    if(_file == nullptr)
    {
        return false;
    }
    return true;
}

// Close the file stream
bool M2Writer::FileClose()
{
    if(_file != nullptr)
    {
        fclose(_file);
        return true;
    }
    return false;
}

// Check whether the file stream is open.
bool M2Writer::IsFileOpen()
{
    return _file != nullptr;
}

// Get the file stream options
void M2Writer::GetWriteOptions()
{
}

// Write file with stream options
bool M2Writer::Write(FbxDocument* document)
{
    if (!document)
    {
        GetStatus().SetCode(FbxStatus::eFailure, "Invalid document handle");
        return false;
    }

    FbxScene* scene = FbxCast<FbxScene>(document);
    if(scene == nullptr) {
        return false;
    }

    PreprocessScene(*scene);
    FBXSDK_printf("I'm in my own writer\n");

    FbxNode* rootNode = scene->GetRootNode();
    PrintHierarchy(rootNode);

    PostprocessScene(*scene);
    return true;
}

// Write out Node Hierarchy recursively
void M2Writer::PrintHierarchy(FbxNode* node)
{
    FbxNode* childNode;
    const char* parentName = node->GetName();
    for(int i = 0; i<node->GetChildCount(); i++)
    {
        childNode = node->GetChild(i);
        FBXSDK_fprintf(_file,"%s%s%s%s%s%s%s","\"",childNode->GetName(),"\"",", parent is ","\"",parentName,"\"\n");
    }

    int nodeChildCount = node->GetChildCount ();
    while (nodeChildCount > 0)
    {
        nodeChildCount--;
        childNode = node->GetChild (nodeChildCount);
        PrintHierarchy(childNode);        
    }
}

// Pre-process the scene before write it out 
bool M2Writer::PreprocessScene(FbxScene& /*pScene*/)
{
    FBXSDK_printf("I'm in pre-process\n");
    return true;
}

// Post-process the scene after write it out 
bool M2Writer::PostprocessScene(FbxScene& /*pScene*/)
{
    FBXSDK_printf("I'm in post process\n");
    return true;
}
