#pragma once

#include <fbxsdk.h>

// functions to be pointed to by fbx plugin registration
FbxWriter* CreateM2Writer(FbxManager& manager, FbxExporter& exporter, int subID, int pluginID);
void* GetM2WriterInfo(FbxWriter::EInfoRequest request, int id);
void FillM2WriterIOSettings(FbxIOSettings& IOS);

//This class is a custom writer.
//The writer provide you the ability to write out node hierarchy to a custom file format.
class M2Writer : public FbxWriter
{
public:
    M2Writer(FbxManager &manager, int id);

    //VERY important to put the file close in the destructor
    virtual ~M2Writer();

    bool FileCreate(char* fileName) override;
    bool FileClose() override;
    bool IsFileOpen() override;
    void GetWriteOptions() override;
    bool Write(FbxDocument* document) override;
    bool PreprocessScene(FbxScene &scene) override;
    bool PostprocessScene(FbxScene &scene) override;

	virtual void PrintHierarchy(FbxNode* startNode);

private:
    FILE*		_file;
    FbxManager*	_manager;
};