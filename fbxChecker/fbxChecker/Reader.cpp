#include "Reader.h"
#include <iostream>

const char* lFileTypes[] =
{
    "_dae.dae",            "Collada DAE (*.dae)",
    "_fbx7binary.fbx", "FBX binary (*.fbx)",
    "_fbx7ascii.fbx",  "FBX ascii (*.fbx)",
    "_fbx6binary.fbx", "FBX 6.0 binary (*.fbx)",
    "_fbx6ascii.fbx",  "FBX 6.0 ascii (*.fbx)",
    "_obj.obj",            "Alias OBJ (*.obj)",
    "_dxf.dxf",            "AutoCAD DXF (*.dxf)"
};


Reader::Reader()
{   
    _sdkManager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(_sdkManager, IOSROOT);
    _sdkManager->SetIOSettings(ios);
    _importer = FbxImporter::Create(_sdkManager, "");
}

Reader::~Reader()
{
    clear();
}

bool Reader::correctFile(const char* filenmame)
{
    if (!_importer->Initialize(filenmame, -1, _sdkManager->GetIOSettings())) {
        std::cout << "Call to FbxImporter::Initialize() failed.\n";
        std::cout << "File not found or wrong name\n";
        std::cout << "Input a valid name.\n";
        return false;
    }
    else
    {
        fixedName = filenmame;
        _scene = FbxScene::Create(_sdkManager, "myScene");
        _importer->Import(_scene);
        _importer->Destroy();
        return true;
    }
}

void Reader::processScene()
{
    Checks checker;
    checker.completeCheck(_scene);
    
    FbxExporter* lExporter = FbxExporter::Create(_sdkManager, "");
    if (fixedName == "Rotated-Scaled.fbx")
    {    
         std::string fixedName = (char*)_scene->GetInitialName();
        fixedName = "Fixed/" + fixedName + "Fixed.fbx";
        int lFormat = _sdkManager->GetIOPluginRegistry()->FindWriterIDByDescription(lFileTypes[7]);
        lNewFileName = fixedName.c_str();
        result = lExporter->Initialize(lNewFileName, lFormat, _sdkManager->GetIOSettings());
        result = lExporter->Export(_scene);
    }
   
}

void Reader::clear()
{
    _sdkManager = nullptr;
    _importer = nullptr;
    _scene = nullptr;

    _sdkManager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(_sdkManager, IOSROOT);
    _sdkManager->SetIOSettings(ios);
    _importer = FbxImporter::Create(_sdkManager, "");
}


