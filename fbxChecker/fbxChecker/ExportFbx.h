#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <string>
#include <vector>

class ExportFbx
{
private:

	std::vector<std::string>fbxNames;
	std::vector<FbxScene*>_scenes;
	FbxExporter* lExporter;
	FbxManager* _sdkManager;
	bool result;
	const char* lNewFileName;

public:
	ExportFbx();
	~ExportFbx();
	void addFbxToFix(std::string fbxName, FbxScene* _scene);
	void exportFbxFixed();
};

