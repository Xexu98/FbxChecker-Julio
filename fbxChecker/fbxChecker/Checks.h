#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <stack>

class Checks {
private:
	//Check Names utils
	bool badName = false, goodName = true;

	std::stack<std::string> badNamesStack;

	std::stack<std::string> badUVsNamesStack;

	int numTabs = 0;

	bool fileFixed = false;

	bool _export;

	bool _error;

	int _lFileMajor, _lFileMinor;
public:
	/**
	Checks the whole scene, and prints the results of the check

	@param scene (FbxScene*) scene that will be checked
	*/
	void completeCheck(FbxScene* scene, bool exFbx, int lFileMajor, int lFileMinor);

	void completeCheck(FbxScene* scene,int lFileMajor, int lFileMinor);

	bool returnError() { return _error; }

private:
	/**
	Checks if the object is a light or a camera

	@param node (FbxNode*) node that will be checked. The children of the node will also be checked

	@return returns a bool: true if there is no light or camera and false if there is either one. This 
	is used to cut the checks in case of a camera/light node
	*/
	bool checkAttributes(FbxNode* node);

	/**
	Checks if the object has correct scale, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkScaling(FbxNode* node);

	/**
	Checks if the object has correct translation, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkTranslation(FbxNode* node);

	/**
	Checks if the object has correct rotation, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkRotation(FbxNode* node);

	/**
	Checks if the object doesn't have lazy names, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkName(const char* node);

	/**
	Checks if the the name of the uv it not lazy

	@param node (string*) name that will be checked
	*/
	bool checkUVsNames(std::string names);

	/**
	Checks if the object has n-gons, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkNgons(FbxNode* node);

	/**
	Checks if the object has normals, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkNormals(FbxNode* node);


	/**
	Checks if the object has UVs, and prints the results of the check

	@param node (FbxNode*) node that will be checked
	*/
	void checkUVs(FbxNode* node);

	/**
	Checks the file version of the actual fbx
	*/
	void checkFileVersion();

	void checkTextures(FbxNode* node);

	/**
	Runs all the individual check on the given node and its children, and prints informationvoid completeCheck(FbxScene* scene);

	@param node (FbxNode*) node that will be checked
	*/
	void processNode(FbxNode* node);

	/**
	Utility method to print tabulations
	*/
	void printTabs();


	bool returnFixed() { return fileFixed; }


};

