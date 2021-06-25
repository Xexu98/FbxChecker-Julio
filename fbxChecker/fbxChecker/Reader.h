#pragma once
#include <iostream>
#include "Checks.h"

class Reader
{
private:
	int numTabs = 0;
	FbxManager* _sdkManager;
	FbxImporter* _importer;
	FbxScene* _scene;
	std::string fixedName;
	bool result;
	const char* lNewFileName ;

public:
	Reader();
	~Reader();

	bool correctFile(const char* filenmame);

	void processScene();

	FbxScene* returnActualScene() { return _scene; }
	//clean and reset the scene
	void clear();
};

