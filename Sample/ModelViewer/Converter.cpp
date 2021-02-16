#include "PreCompile.h"
#include "Global.h"


#define SCALE_X (1.0f)
#define SCALE_Y (1.0f)
#define SCALE_Z (1.0f)

float minY = FLT_MAX;
float minX = FLT_MAX;
float maxX = -FLT_MAX;
float minZ = FLT_MAX;
float maxZ = -FLT_MAX;

#define TRANS_TO_VECTOR(v) (Vector3Df(v.mData[0], v.mData[1], v.mData[2]))

Converter::Converter()
{
    
}

Converter::~Converter()
{
    
}

Matrix4 Converter::GetFbxNodeTransform(FbxNode* node)
{
    FbxAMatrix& globalMatrix = node->EvaluateGlobalTransform();

    FbxAMatrix matrixGeo;
    matrixGeo.SetIdentity();

    if(node->GetNodeAttribute())
    {
        const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
        const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
        const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);

        matrixGeo.SetT(lT);
        matrixGeo.SetR(lR);
        matrixGeo.SetS(lS);
    }

    FbxAMatrix transformMatrix = globalMatrix * matrixGeo;
    double* m = (double*)transformMatrix;

    Matrix4 outTrans;
    for (int i = 0; i < 16; i++)
    {
        outTrans.m[i] = m[i];
    }
        
    Matrix4 tran;
    tran.SetRotationDegrees(Vector3Df(0, -90.0f, 0));
    

    return outTrans * tran;
}

Vector3Df Converter::GetFbxMeshNormal(FbxGeometryElementNormal* normals, int ctrlPointIndex, int vertCounter)
{
    FbxLayerElement::EMappingMode normalMapMode = normals->GetMappingMode();
    FbxLayerElement::EReferenceMode normalReferMode = normals->GetReferenceMode();
    FbxLayerElementArrayTemplate<int>& indexArray = normals->GetIndexArray();
    FbxLayerElementArrayTemplate<FbxVector4>& directArray = normals->GetDirectArray();

    int directIndex = 0;
    if(normalMapMode == FbxLayerElement::eByControlPoint && normalReferMode == FbxLayerElement::eDirect)
    {
        directIndex = ctrlPointIndex;
    }
    else if(normalMapMode == FbxLayerElement::eByControlPoint && normalReferMode == FbxLayerElement::eIndexToDirect)
    {
        directIndex = indexArray.GetAt(ctrlPointIndex);
    }
    else if(normalMapMode == FbxLayerElement::eByPolygonVertex && normalReferMode == FbxLayerElement::eDirect)
    {
        directIndex = vertCounter;
    }
    else if(normalMapMode == FbxLayerElement::eByPolygonVertex && normalReferMode == FbxLayerElement::eIndexToDirect)
    {
        directIndex = indexArray.GetAt(vertCounter);
    }

    return TRANS_TO_VECTOR(directArray.GetAt(directIndex));
}

void Converter::ProcessFbxMesh(FbxMesh* mesh, std::vector<Vertex>& vertList, std::vector<int>& indicesList, Matrix4& transMatrix)
{
    if(!mesh->IsTriangleMesh())
    {
        return;
    }

    if (mesh->GetElementNormalCount() > 1)
    {
        return;
    }

	int ctrlPointCount = mesh->GetControlPointsCount();
	int triCount = mesh->GetPolygonCount();
	FbxVector4* ctrlPoints = mesh->GetControlPoints();
    FbxGeometryElementNormal* normals = mesh->GetElementNormal();

	for (int i = 0; i < ctrlPointCount; i++)
	{
        Vector3Df pos = TRANS_TO_VECTOR(ctrlPoints[i]);

        Vector3Df transPos = transMatrix.TransformPoint(pos);

        minY = Math::minimum(transPos.y, minY);
        minX = Math::minimum(transPos.x, minX);
        maxX = Math::maximum(transPos.x, maxX);
        minZ = Math::minimum(transPos.z, minZ);
        maxZ = Math::maximum(transPos.z, maxZ);

		Vertex vt;
        vt.pos = transPos;

		vertList.push_back(vt);
	}

	for (int i = 0; i < triCount; i++)
	{
        for (int j = 0; j < 3; j++)
        {
            int ctrlPointIndex = mesh->GetPolygonVertex(i, j);

            indicesList.push_back(ctrlPointIndex);

            vertList[ctrlPointIndex].normal += GetFbxMeshNormal(normals, ctrlPointIndex, i * 3 + j);
        }
	}

    for(int j = 0; j < vertList.size(); j++)
    {
        Vector3Df normal = transMatrix.TransformVector(vertList[j].normal);
        vertList[j].normal = normal.Normalize();
    }
}

bool Converter::OpenFbx(const char8* path)
{
	FbxManager* sdkManager = FbxManager::Create();
    FbxScene* scene = FbxScene::Create(sdkManager, "my scene");
    FbxImporter* importer = FbxImporter::Create(sdkManager, "");
    bool status = importer->Initialize(path, -1, sdkManager->GetIOSettings());
    if (!status)
    {
        sdkManager->Destroy();
        return false;
    }

	importer->Import(scene);

    FbxGeometryConverter converter(sdkManager);
    converter.Triangulate(scene, true);

	FbxNode* rootNode = scene->GetRootNode();
    int childCount = rootNode->GetChildCount();
    m_meshInfos.resize(childCount);
    
    for (int i = 0; i < childCount; i++)
    {
        FbxNode* childNode = rootNode->GetChild(i);
        const char* nodeName = childNode->GetName();
        
        printf("node name:%s\n", nodeName);
        
        m_meshInfos[i].name = nodeName;
        std::vector<Vertex>& vertList = m_meshInfos[i].vertList;
        std::vector<int>& indicesList = m_meshInfos[i].indicesList;

        Matrix4 nodeTrans = GetFbxNodeTransform(childNode);

        FbxNodeAttribute* attribute = childNode->GetNodeAttribute();
        if (attribute != NULL)
        {
            FbxNodeAttribute::EType type = attribute->GetAttributeType();
            if(type == FbxNodeAttribute::eMesh)
            {
                FbxMesh* childMesh = childNode->GetMesh();
                if (childMesh != NULL)
                {
                    ProcessFbxMesh(childMesh, vertList, indicesList, nodeTrans);

                    printf("vertex num:%d face num:%d\n", (int)vertList.size(), (int)indicesList.size() / 3);
                }
            }
        }
    }

    sdkManager->Destroy();

	return true;
}

XmlNode* Converter::AppendVectorNode(XmlNode* parent, const char8* name, const Vector3Df& v)
{
    XmlNode* vectorNode = parent->AppendChild(name);
    
    XmlAttribute* vectorAttr = vectorNode->AppendAttribute("v");
    
    char buf[255];
    sprintf(buf, "%.6f %.6f %.6f", v.x, v.y, v.z);
    vectorAttr->SetValue(buf);
    
    return vectorNode;
}

bool Converter::Save(const char8* path)
{
    if (m_name.empty()) return false;
    
    XmlDocument* doc = (XmlDocument*)sResMgr->CreateResource(ERT_XML, path);
    if (doc == nullptr) return false;
    
    XmlNode* rootNode = doc->AppendRootNode("Scene");
    XmlNode* materialChunkNode = rootNode->AppendChild("materials_chunk");
    XmlNode* meshChunkNode = rootNode->AppendChild("meshes_chunk");
    XmlNode* modelChunkNode = rootNode->AppendChild("models_chunk");
    XmlNode* nodeChunkNode = rootNode->AppendChild("nodes_chunk");
    
    for(uint32 i = 0; i < m_meshInfos.size(); i++)
    {
        XmlNode* meshNode = meshChunkNode->AppendChild("mesh");
        XmlAttribute* nameAttr = meshNode->AppendAttribute("name");
        nameAttr->SetValue(m_meshInfos[i].name.c_str());
        
        XmlNode* verticesChunkNode = meshNode->AppendChild("vertices_chunk");
        
        for (Vertex& vert : m_meshInfos[i].vertList)
        {
            XmlNode* vertexNode = AppendVectorNode(verticesChunkNode, "vertex", vert.pos);
            
            AppendVectorNode(vertexNode, "normal", vert.normal);
        }
        
        XmlNode* trianglesChunkNode = meshNode->AppendChild("triangles_chunk");
        vector<int>& indicesList = m_meshInfos[i].indicesList;
        
        char buf[128];
        for (uint32 j = 0; j < indicesList.size() / 3; j++)
        {
            int index1 = indicesList[j * 3 + 0];
            int index2 = indicesList[j * 3 + 1];
            int index3 = indicesList[j * 3 + 2];
            sprintf(buf, "%d %d %d", index1, index2, index3);
            
            XmlNode* triangleNode = trianglesChunkNode->AppendChild("triangle");
            XmlAttribute* indexAttr = triangleNode->AppendAttribute("index");
            indexAttr->SetValue(buf);
        }
    }
    
    XmlNode* materialNode = materialChunkNode->AppendChild("material");
    XmlAttribute* materialNameAttr = materialNode->AppendAttribute("name");
    materialNameAttr->SetValue("Paint");
    XmlAttribute* materialTechAttr = materialNode->AppendAttribute("technique");
    materialTechAttr->SetValue("Sprite3DNormal");
    
    XmlNode* modelNode = modelChunkNode->AppendChild("model");
    XmlAttribute* modelNameAttr = modelNode->AppendAttribute("name");
    modelNameAttr->SetValue(m_name.c_str());
    for (auto& meshInfo : m_meshInfos)
    {
        XmlNode* meshItemNode = modelNode->AppendChild("item");
        XmlAttribute* meshTypeAttr = meshItemNode->AppendAttribute("type");
        meshTypeAttr->SetValue("mesh");
        XmlAttribute* meshIdAttr = meshItemNode->AppendAttribute("id");
        meshIdAttr->SetValue(meshInfo.name.c_str());
        
        XmlNode* materialItemNode = meshItemNode->AppendChild("item");
        XmlAttribute* materialTypeAttr = materialItemNode->AppendAttribute("type");
        materialTypeAttr->SetValue("material");
        XmlAttribute* materialIdAttr = materialItemNode->AppendAttribute("id");
        materialIdAttr->SetValue("Paint");
    }
    
    XmlNode* nodeNode = nodeChunkNode->AppendChild("model");
    XmlAttribute* nodeNameAttr = nodeNode->AppendAttribute("name");
    string nodeName = m_name + "Node";
    nodeNameAttr->SetValue(nodeName.c_str());
    
    XmlAttribute* transAttr = nodeNode->AppendAttribute("transform");
    transAttr->SetValue("1.000 0.000 0.000 0.000 0.000 1.000 0.000 0.000 0.000 0.000 1.000 0.000 0.000 0.000 0.000 1.000");
    
    XmlAttribute* boundingRadiusAttr = nodeNode->AppendAttribute("bounding_radius");
    boundingRadiusAttr->SetValue("0.000000");
    
    
    
    XmlNode* modelItemNode = nodeNode->AppendChild("item");
    XmlAttribute* modelTypeAttr = modelItemNode->AppendAttribute("type");
    modelTypeAttr->SetValue("model");
    
    XmlAttribute* modelIdAttr = modelItemNode->AppendAttribute("id");
    modelIdAttr->SetValue(m_name.c_str());
    
    doc->Save(path);
    
    return false;
}
