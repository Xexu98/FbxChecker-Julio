#include "ExportFbx.h"



ExportFbx::ExportFbx()
{
	_sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(_sdkManager, IOSROOT);
	_sdkManager->SetIOSettings(ios);
	lExporter = FbxExporter::Create(_sdkManager, "");
}

ExportFbx::~ExportFbx()
{

}

void ExportFbx::addFbxToFix(std::string fbxName, FbxScene* _scene)
{
	fbxNames.push_back(fbxName);
	_scenes.push_back(_scene);
}

void ExportFbx::exportFbxFixed()
{

	/*std::string fixedName = (char*)_scene->GetInitialName();
	fixedName = "Fixed/" + fixedName + "Fixed.fbx";
	int lFormat = _sdkManager->GetIOPluginRegistry()->FindWriterIDByDescription(lFileTypes[7]);
	lNewFileName = fixedName.c_str();
	result = lExporter->Initialize(lNewFileName, lFormat, _sdkManager->GetIOSettings());
	result = lExporter->Export(_scene);*/
}


