#include "Checks.h"
#include <filesystem>
#include <fstream>
#include <string>
#include "Output.h"

//C++17
namespace fs = std::filesystem;

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch (type) {
    case FbxNodeAttribute::eUnknown: return "unidentified";
    case FbxNodeAttribute::eNull: return "null";
    case FbxNodeAttribute::eMarker: return "marker";
    case FbxNodeAttribute::eSkeleton: return "skeleton";
    case FbxNodeAttribute::eMesh: return "mesh";
    case FbxNodeAttribute::eNurbs: return "nurbs";
    case FbxNodeAttribute::ePatch: return "patch";
    case FbxNodeAttribute::eCamera: return "camera";
    case FbxNodeAttribute::eCameraStereo: return "stereo";
    case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
    case FbxNodeAttribute::eLight: return "light";
    case FbxNodeAttribute::eOpticalReference: return "optical reference";
    case FbxNodeAttribute::eOpticalMarker: return "marker";
    case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
    case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
    case FbxNodeAttribute::eBoundary: return "boundary";
    case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
    case FbxNodeAttribute::eShape: return "shape";
    case FbxNodeAttribute::eLODGroup: return "lodgroup";
    case FbxNodeAttribute::eSubDiv: return "subdiv";
    default: return "unknown";
    }
}


bool Checks::checkAttributes(FbxNode* node) {
    bool isMesh = true; 
    for (int i = 0; i < node->GetNodeAttributeCount(); i++) {
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        FbxString typeName = GetAttributeTypeName(attribute->GetAttributeType());
        FbxString attrName = attribute->GetName();
        printTabs();

        if (typeName == "camera" || typeName == "light") {
            isMesh = false;        
            FbxCamera* auxC= node->GetCamera();
            if (auxC != nullptr)
            {
                _error = true;
                auxC->Destroy();
            }
            FbxLight* auxL= node->GetLight();
            if (auxL != nullptr)
            {
                _error = true;
                auxL->Destroy();
            }
            
            break;
        }
    }
    
        // TODO: fix this for child nodes too
    for (int j = 0; j < node->GetChildCount(); j++) {
         isMesh &= checkAttributes(node->GetChild(j));
    }
    

    if (!isMesh) {
        std::string f = node->GetName();
        f += " is a light and/or camera";
        if (!_export)
        {
            Output::newFbxProblem(2, f);
        }
        else
        {
            _error = true;
        }
    }

    return isMesh;
}

void Checks::checkScaling(FbxNode* pNode) {
    const char* nodeName = pNode->GetName();
    fbxsdk::FbxDouble3 scaling = pNode->LclScaling.Get();
   
    // Check if the scale is equal in all axis
    if (scaling[0] == scaling[1] && scaling[0] == scaling[2]) {

        // Scale is equal to 1
        if (!((scaling[0] <= 1.001 && scaling[0] >= 1)
            && (scaling[1] <= 1.001 && scaling[1] >= 1)
            && (scaling[2] <= 1.001 && scaling[2] >= 1))) {
           
            if (!_export)
            {
                Output::newFbxProblem(1, "Scale is equal in all axis but differs from unit= (" + std::to_string(scaling[0]) + ", " + std::to_string(scaling[1]) + ", " + std::to_string(scaling[2]) + ")");
            }
            else
            {
                _error = true;
                pNode->LclScaling.Set(FbxDouble3(1, 1, 1));
            }
            return;
        }
    }
    else {
       
        if (!_export)
        {
            Output::newFbxProblem(2, "Scale is different in axis= (" + std::to_string(scaling[0]) + ", " + std::to_string(scaling[1]) + ", " + std::to_string(scaling[2]) + ")");
        }
        else
        {
            _error = true;
            pNode->LclScaling.Set(FbxDouble3(1, 1, 1));
        }
        return;
    }
}

void Checks::checkTranslation(FbxNode* pNode) {
    const char* nodeName = pNode->GetName();
    fbxsdk::FbxDouble3 translation = pNode->LclTranslation.Get();
    // Check if the translation is equal 0 in all axis
    if ( !((translation[0] <= 0.001 && translation[0] >= -0.001) 
        && (translation[1] <= 0.001 && translation[1] >= -0.001) 
        && (translation[2] <= 0.001 && translation[2] >= -0.001))) {
       
        if (!_export)
        {
            _error = true;
            Output::newFbxProblem(1, "Translation is not equal to zero= (" + std::to_string(translation[0]) + ", " + std::to_string(translation[1]) + ", " + std::to_string(translation[2]) + ")");
        }
        else
        {
            _error = true;
            pNode->LclTranslation.Set(FbxDouble3(0, 0, 0));
        }
    }
}

void Checks::checkRotation(FbxNode* pNode) {
    fbxsdk::FbxDouble3 rotation = pNode->LclRotation.Get();
    const char* nodeName = pNode->GetName();

    // initialized to true to be able to use logic & on itself
    bool ok = true;
    // initialized to false to be able to use logic | on itself
    bool exportRotation = false;

    for (int i = 0; i < 3; i++) {
        ok &= abs(rotation[i]) < 0.01;
        int rotInt = floor(abs(rotation[i]) / 90.0);
        double rotDou = (abs(rotation[i]) / 90.0);
        // rotation in the axis != 0 and multiple of 90
        exportRotation |= abs(rotation[i]) > 0.01 && abs(rotInt - rotDou) < 0.0001;
    }

    if (ok) {
    }
    else if (exportRotation){
        
        if (!_export)
        {
            Output::newFbxProblem(1, "Possible residual export rotations= (" + std::to_string(rotation[0]) + ", " + std::to_string(rotation[1]) + ", " + std::to_string(rotation[2]) + ")");
        }
        else
        {
            _error = true;
            pNode->LclRotation.Set(FbxDouble3(0, 0, 0));
        }
    }
    else {
        if (!_export)
        {
            
            Output::newFbxProblem(2, "Rotation is different in some axis= (" + std::to_string(rotation[0]) + ", " + std::to_string(rotation[1]) + ", " + std::to_string(rotation[2]) + ")");
        }
        else
        {
            _error = true;
            pNode->LclRotation.Set(FbxDouble3(0, 0, 0));
        }
    }
}


void Checks::checkName(const char* nNode) {
    //We use this as a string because is more easy :D
    std::string name = nNode, util;
    //if the stack is empty read the text and 
    if (badNamesStack.empty()) {
        char textChain[128];
        std::ifstream fe(fs::current_path().root_name().string() + "BadNames.txt");
        if (fe.is_open()) {
            while (!fe.eof()) {
                fe >> util;
                badNamesStack.push(util);
            }
            fe.close();
        }
    }


    //Save the stack in other, we dont need to read the file 
    //more than one time
    std::stack<std::string> saveStack;
    bool actualBadName = false;
    while (!badNamesStack.empty() && !actualBadName) {
        if (name.find(badNamesStack.top()) != std::string::npos) {
            if (!_export)
            {
                Output::newFbxProblem(1, name + " is a bad name, please change it");
            }
            goodName = false;
            actualBadName = true;
        }
        saveStack.push(badNamesStack.top());
        badNamesStack.pop();
    }
    if (!actualBadName) {
        badName = false;
    }
    while (!saveStack.empty()) {
        badNamesStack.push(saveStack.top());
        saveStack.pop();
    }
}

bool Checks::checkUVsNames(std::string names)
{
    std::string name = names, util;
    if (badUVsNamesStack.empty()) {
        char textChain[128];
        std::ifstream fe(fs::current_path().root_name().string() + "BadUVsNames.txt");
        if (fe.is_open()) {
            while (!fe.eof()) {
                fe >> util;
                badUVsNamesStack.push(util);
            }
            fe.close();
        }
    }

    std::stack<std::string> saveStack;
    bool actualBadName = false;
    while (!badUVsNamesStack.empty() && !actualBadName) {
        if (name.find(badUVsNamesStack.top()) != std::string::npos) {
            goodName = false;
            actualBadName = true;
        }
        saveStack.push(badUVsNamesStack.top());
        badUVsNamesStack.pop();
    }
    return actualBadName;
}



void Checks::checkNgons(FbxNode* node) {
    const char* nodeName = node->GetName();
    // stores max number of vertices in poly
    int maxVertices = 0;

    //get mesh
    FbxMesh* mesh = node->GetMesh();

    if (mesh) {
        // check for polygon size 
        for (int polyIndex = 0; polyIndex < mesh->GetPolygonCount(); polyIndex++) {
            //get polygon size and if it's bigger than the stored one, replace
            (mesh->GetPolygonSize(polyIndex) > maxVertices) ? 
                maxVertices = mesh->GetPolygonSize(polyIndex) : maxVertices;
        }
    } // if (mesh)

    switch (maxVertices) {
    case 3:
        break;
    case 4:
        if (!_export)
        {
            Output::newFbxProblem(1, "Some quads present");
        }
        break;
    default:
        if (!_export)
        {
            Output::newFbxProblem(2, " there are N-gons in the mesh");
        }
        break;
    }
}

void Checks::checkNormals(FbxNode* node) {
    const char* nodeName = node->GetName();

    //get mesh
    FbxMesh* mesh = node->GetMesh();
    if (mesh) {
        FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();

        if (!normalElement) {
            if (!_export)
            {
                Output::newFbxProblem(1, "object does not have normals");
            }
            else
            {
                _error = true;
            }
        }
    }
}

void Checks::checkUVs(FbxNode* node)
{
    FbxMesh* mesh = node->GetMesh();
    FbxStringList lUVSetNameList;
    mesh->GetUVSetNames(lUVSetNameList);
   

    if (lUVSetNameList.GetCount() == 0 )
    {
        if (!_export)
        {
            Output::newFbxProblem(2, "No UVs assigned");
        }
        else
        {
            _error = true;
            //mesh->CreateElementUV("UVMaping");
        } 
    }
    else
    {
        
        int numBadNames=0; 
       
        for (int i = 0; i < lUVSetNameList.GetCount(); i++)
        {      
            if(checkUVsNames(lUVSetNameList.GetStringAt(i)))
            {
                numBadNames++;
            }
           
        }
         if (!_export && numBadNames > 0)
        {
             std::string salida = " There are " + std::to_string(numBadNames);
             salida = salida + " UVs bad names";
              Output::newFbxProblem(1, salida);
        }
    }
}

void Checks::checkFileVersion()
{
    if (!_export && _lFileMajor <= 6 && _lFileMinor <= 5 )
    {
        Output::newFbxProblem(1, "The file version is 6.0 or less this can generate problems in some programs");
    }
}

void Checks::checkTextures(FbxNode* node) {
    if (node->GetMaterialCount() == 0) {
        if (!_export)
        {
            Output::newFbxProblem(1, "No material assigned, object cannot have texture");
        }
        else
        {
            _error = true;
            FbxString lMaterialName = "mate";
            FbxSurfacePhong* mat = FbxSurfacePhong::Create(node->GetScene(), lMaterialName.Buffer());;
            node->AddMaterial(mat);
        } 
        return;
    }
    
    bool textureExists = false;

    for (int i = 0; i < node->GetMaterialCount(); i++) {

        FbxSurfaceMaterial* material = node->GetMaterial(i);
        FbxProperty prop;
        int textureIndex;

        FBXSDK_FOR_EACH_TEXTURE(textureIndex) {
            prop = material->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
            if (prop.IsValid()) {
                int totalTextures = prop.GetSrcObjectCount<FbxTexture>();

                textureExists |= (totalTextures != 0);

                for (int currentTexture = 0; currentTexture < totalTextures; ++currentTexture) {
                    FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(currentTexture);

                    if (layeredTexture) {
                        FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(currentTexture);
                        int numTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
                        for (int k = 0; k < numTextures; ++k) {
                            FbxTexture* texture = layeredTexture->GetSrcObject<FbxTexture>(k);
                            if (!texture) {
                                if (!_export)
                                {
                                    Output::newFbxProblem(1, "Some textures missing");
                                }
                                else
                                {
                                    _error = true;
                                }
                            }
                        }
                    } // if(layeredTexture)
                    else {
                        FbxTexture* texture = prop.GetSrcObject<FbxTexture>(currentTexture);
                        if (!texture) {
                            if (!_export)
                            {
                                Output::newFbxProblem(1, "Some textures missing");
                            }
                            else
                            {
                                _error = true;
                            }
                        }
                    } // else
                }// for(texture loop)
            } // if(prop.isValid())
        } //FBXSDK_FOR_EACH_TEXTURE

        if (!textureExists) {
            if (!_export)
            {
                Output::newFbxProblem(1, "Some textures missing");
            }
            else
            {
                _error = true;
            }
        }
    }
}


void Checks::completeCheck(FbxScene* scene, bool exFbx, int lFileMajor, int lFileMinor) {
    FbxNode* rootNode = scene->GetRootNode();
    _export = exFbx;
    if (rootNode) {
        _lFileMajor = lFileMajor;
        _lFileMinor = lFileMinor;

        for (int i = 0; i < rootNode->GetChildCount(); i++) {
            processNode(rootNode->GetChild(i));
        }

    }
}

void Checks::completeCheck(FbxScene* scene, int lFileMajor, int lFileMinor)
{
    FbxNode* rootNode = scene->GetRootNode();
    if (rootNode) {
        _lFileMajor = lFileMajor;
        _lFileMinor = lFileMinor;

        for (int i = 0; i < rootNode->GetChildCount(); i++) {
            processNode(rootNode->GetChild(i));
        }
    
    }
}

void Checks::processNode(FbxNode* node) {
    printTabs();
    _error = false;
    const char* nodeName = node->GetName();
    fbxsdk::FbxDouble3 translation = node->LclTranslation.Get();
    fbxsdk::FbxDouble3 rotation = node->LclRotation.Get();
    fbxsdk::FbxDouble3 scaling = node->LclScaling.Get();

    numTabs++;

    bool isMesh = checkAttributes(node);

    if (isMesh && node->GetNodeAttributeCount() != 0) {
        checkTranslation(node);
        checkScaling(node);
        checkRotation(node);
        checkName(node->GetName());
        checkNgons(node);
        checkNormals(node);
        checkUVs(node);
        checkTextures(node);

        for (int j = 0; j < node->GetChildCount(); j++) {
            processNode(node->GetChild(j));
            checkTranslation(node->GetChild(j));
            checkScaling(node->GetChild(j));
            checkRotation(node->GetChild(j));
            checkName(node->GetChild(j)->GetName());
            checkNormals(node->GetChild(j));
            checkNgons(node->GetChild(j));
            checkUVs(node->GetChild(j));
            checkTextures(node->GetChild(j));
        }
    }
    else
    {
        node->Destroy();
    }
    numTabs--;
    printTabs();
}


void Checks::printTabs() {
    for (int i = 0; i < numTabs; i++)
        printf("\t");
}
