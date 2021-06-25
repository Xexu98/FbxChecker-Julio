#include "ExportFbx.h"
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

ExportFbx::ExportFbx()
{
	_sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(_sdkManager, IOSROOT);
	_sdkManager->SetIOSettings(ios);
	lExporter = FbxExporter::Create(_sdkManager, "");
}

ExportFbx::~ExportFbx()
{
	_sdkManager = nullptr;
	lExporter = nullptr;
}

void ExportFbx::addFbxToFix(std::string fbxName, FbxScene* _scene)
{
	fbxNames.push_back(fbxName);
	_scenes.push_back(_scene);
}

void ExportFbx::exportFbxFixed()
{
	for (int i = 0; i < fbxNames.size(); i++)
	{
		std::string fixedName = fbxNames[i];
		fixedName = "Fixed/" + fixedName + "Fixed.fbx";
		int lFormat = _sdkManager->GetIOPluginRegistry()->FindWriterIDByDescription(lFileTypes[7]);
		lNewFileNames = fixedName.c_str();
		result = lExporter->Initialize(lNewFileNames, lFormat, _sdkManager->GetIOSettings());
		result = lExporter->Export(_scenes[i]);
	}
	
}


