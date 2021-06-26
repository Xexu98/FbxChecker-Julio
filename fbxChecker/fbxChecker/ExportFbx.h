#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>

class ExportFbx
{
private:

	std::vector<std::string>fbxNames;
	std::vector<FbxScene*>_scenes;
	FbxExporter* lExporter;
	FbxManager* _sdkManager;
	bool result;
	const char* lNewFileNames;

public:
	ExportFbx();
	~ExportFbx();
	void assingSdkManager(FbxManager* sM);
	void addFbxToFix(std::string fbxName, FbxScene* _scene);
	void exportFbxFixed();
};

