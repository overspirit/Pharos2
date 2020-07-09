#include "PreCompile.h"
#include "Global.h"

MapNode::MapNode()
{
}

MapNode::~MapNode()
{
}

bool MapNode::LoadMap(const char8* path)
{
	File* mapFile = sResMgr->CreateResourceFile(path);
	if (mapFile == nullptr) return false;

	uint32 fileSize = mapFile->GetSize();
	MemoryBuffer membuf;
	membuf.Alloc(fileSize + 1);
	char8* dataBuf = (char*)membuf.GetPointer();
	dataBuf[fileSize] = 0;
	mapFile->Read(dataBuf, fileSize);
	SAFE_DELETE(mapFile);

	rapidjson::Document doc;
	doc.Parse(dataBuf);
	if(doc.HasParseError())
	{
		rapidjson::ParseErrorCode errorCode = doc.GetParseError();
		size_t errorOffset = doc.GetErrorOffset();

		return false;
	}
	
	rapidjson::Value& rootData = doc["Data"];
	rapidjson::Value& regionsData = rootData["SubRegions"];
	
	Material* gridMaterial = sMaterialMgr->GenerateMaterial("Sprite3DColor");
	
	Model* gridModel = new Model();

	vector<VertLayoutDesc> vertDesc;
	vertDesc.push_back({ VET_FLOAT32, 3, VAL_POSITION, 0, 0 });
	vertDesc.push_back({ VET_UNORM8, 4, VAL_COLOR, 12, 0 });

	Vector3Df	center;
	uint32 totalPosNum = 0;

	vector<Vector3Df>	firstRegion;

	for (uint32 i = 0; i < regionsData.Size(); i++)
	{
		rapidjson::Value& region = regionsData[i];
		rapidjson::Value& points = region["Geometry"][0][0];
		
		float32 p = points[2].GetFloat();
		if (points.Size() >= 3 &&  (p < 0 || p > 0))
		{
			continue;
		}		

		vector<Vector3Df>	linePosList;

		for (uint32 j = 0; j < points.Size(); j+=3)
		{
			Vector3Df pos;
			pos.x = points[j].GetFloat();
			pos.z = points[j + 1].GetFloat();
			pos.y = points[j + 2].GetFloat();

			linePosList.push_back(pos);

			center += pos;
			totalPosNum += 1;

			//printf("pos x:%f y:%f z:%f\n", pos.x, pos.y, pos.z);
		}

		//临时，找寻0车位
		if (i == 0)
		{
			firstRegion = linePosList;
		}
		

		MemoryBuffer vertexData;
		vertexData.Alloc(linePosList.size() * sizeof(Vertex));

		for (uint32 i = 0; i < linePosList.size(); i++)
		{
			Vertex vt = { linePosList[i], 0xFFCCCCCC };
			vertexData.Insert(i * sizeof(Vertex), &vt, sizeof(Vertex));
		}

		Mesh* gridMesh = new Mesh();
		gridMesh->SetMeshVertexData(&vertexData, linePosList.size(), vertDesc);
		gridMesh->SetDrawType(Render::EDT_LINESTRIP);

		uint32 subModelIndex = gridModel->AddSubModelMesh(gridMesh);
		gridModel->AddSubModelMaterial(subModelIndex, gridMaterial);
	}

	center /= totalPosNum;

	Matrix4 rotation;
	rotation.SetRotationDegrees(Vector3Df(0, -74.2f, 0));

	Matrix4 translatin;
	translatin.SetTranslation(-center);

	m_localTransform = translatin * rotation;

	m_modelList.push_back(gridModel);

	
	Vector3Df vt1 = m_localTransform.TransformPoint(firstRegion[0]);
	Vector3Df vt2 = m_localTransform.TransformPoint(firstRegion[1]);
	Vector3Df vt3 = m_localTransform.TransformPoint(firstRegion[2]);
	Vector3Df vt4 = m_localTransform.TransformPoint(firstRegion[3]);

	Vector3Df firstPos = (vt1 + vt2 + vt3 + vt4) / 4;
	m_regionList.push_back(firstPos);


	m_renderObj = sRenderMgr->GenerateRenderObject();
	
	return true;
}