#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"

XmlSceneImporter::XmlSceneImporter()
{

}

XmlSceneImporter::~XmlSceneImporter(void)
{

}

bool XmlSceneImporter::LoadSceneFile(const char8* file)
{
	XmlDocument doc;
	if (!doc.Open(file)) return false;
	if (!doc.Load()) return false;

	XmlNode* rootNode = doc.GetRootNode();

	for (uint32 i = 0; i < rootNode->GetChildNum(); i++)
	{
		XmlNode* nodeNode = rootNode->GetChildNode(i);

		XmlAttribute* radiusAttr = nodeNode->GetAttribute("bs_radius");
		if(radiusAttr != nullptr) float32 nodeRadius = radiusAttr->GetFloatValue();

		XmlAttribute* posAttr = nodeNode->GetAttribute("pos");
		if(posAttr != nullptr) Vector3Df nodePos = posAttr->GetVector3DValue();

// 		IEntityBodyPtr entityNode = sSceneMgr->CreateEntityBody();
// 		entityNode->SetPostion(nodePos);
// 		entityNode->SetBoundingRadius(nodeRadius);

		for (uint32 j = 0; j < nodeNode->GetChildNum(); j++)
		{
			XmlNode* modelNode = nodeNode->GetChildNode(j);

			XmlAttribute* fileAttr = modelNode->GetAttribute("file");
			const char8* objFile = fileAttr->GetStringValue();

			XmlAttribute* offsetAttr = modelNode->GetAttribute("offset");
			Vector3Df modelOffsetPos = offsetAttr->GetVector3DValue();

			Matrix4 objOffsetMat;
			objOffsetMat.SetTranslation(modelOffsetPos);

			//entityNode->LoadModel(objFile);
		}

		//this->AddSceneNode(entityNode);
	}

	return true;
}

bool XmlSceneImporter::LoadModelFile(const char8* file)
{
	m_resFile = file;

	XmlDocument xmlDoc;
	if (!xmlDoc.Open(file)) return false;
	if (!xmlDoc.Load()) return false;

	XmlNode* rootNode = xmlDoc.GetRootNode();
	if (rootNode == nullptr) return false;

	if (!ReadMaterial(rootNode)) return false;
	if (!ReadMesh(rootNode)) return false;
	if (!ReadBoneInfo(rootNode)) return false;
	if (!ReadAnimation(rootNode)) return false;

	return true;
}

bool XmlSceneImporter::ReadMaterial(XmlNode* rootNode)
{
	//读取材质信息
	//////////////////////////////////////////////////////////////////////////
	XmlNode* materialListNode = rootNode->GetFirstNode("materials_chunk");
	if (materialListNode != nullptr)
	{
		uint32 materialNum = materialListNode->GetChildNum();

		//m_materialDataList.resize(materialNum);

		Utils::Path resPath(m_resFile.c_str());
		string resFilePath = resPath.GetFullPath();

		for (uint32 i = 0; i < materialNum; i++)
		{
			XmlNode* materialNode = materialListNode->GetChildNode(i);

			for (uint32 j = 0; j < materialNode->GetChildNum(); j++)
			{
				XmlNode* pTexNode = materialNode->GetChildNode(j);
				XmlAttribute* texTypeAttr = pTexNode->GetAttribute("type");
				string texType = texTypeAttr->GetStringValue();

				XmlAttribute* texFileAttr = pTexNode->GetAttribute("name");
				string texFile = resFilePath + texFileAttr->GetStringValue();

				if (texType == "Diffuse Color")
				{
					//m_materialDataList[i].diffTexPath = texFile;
				}
				else if (texType == "Bump")
				{
					//m_materialDataList[i].bumpTexPath = texFile;
				}
				else if (texType == "Specular Level")
				{
					//m_materialDataList[i].specTexPath = texFile;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return true;
}

template<typename T>
bool ReadArray(XmlNode* node, const char8* attrName, T* array, int32 num)
{
	if (node == nullptr || attrName == nullptr || array == nullptr) return false;

	XmlAttribute* attr = node->GetAttribute(attrName);
	if (attr == nullptr) return false;

	const char8* data = attr->GetStringValue();
	char8* end = nullptr;

	for (int32 i = 0; i < num; i++)
	{
		if (*data == '\0') return false;

		array[i] = (T)strtod(data, &end);
		data = end;
	}

	return true;
}

bool XmlSceneImporter::ReadMesh(XmlNode* rootNode)
{	
	//读取网格信息
	//////////////////////////////////////////////////////////////////////////
	XmlNode* meshListNode = rootNode->GetFirstNode("meshes_chunk");
	
	uint32 meshNum = meshListNode->GetChildNum();
	m_modelData.meshDataList.resize(meshNum);

	//注意！！根据可以读取顶点的信息修改大小
	MemoryBuffer tempVertData;
	tempVertData.Alloc(128);
	
	for (uint32 i = 0; i < meshNum; i++)
	{
		XmlNode* meshNode = meshListNode->GetChildNode(i);
		
		vector<VertLayoutDesc> vertexDesc;
		uint32 stride = 0;

		//读取顶点信息
		//////////////////////////////////////////////////////////////////////////
		XmlNode* pVertListNode = meshNode->GetFirstNode("vertices_chunk");
		uint32 vertNum = pVertListNode->GetChildNum();		

		for (uint32 j = 0; j < vertNum; j++)
		{
			XmlNode* pVertNode = pVertListNode->GetChildNode(j);
			const char8* name = pVertNode->GetName();
			if (strcmp(name, "vertex") == 0)
			{
				vector<VertLayoutDesc> currVertDesc;
				uint32 offset = 0;

				Vector3Df pos;
				ReadArray(pVertNode, "v", (float32*)pos, 3);
				tempVertData.Insert(offset, &pos, sizeof(pos));
				currVertDesc.push_back({ Render::VET_FLOAT32, 3, "POSITION", 0, offset });
				offset += 12;

				XmlNode* normalNode = pVertNode->GetFirstNode("normal");
				if (normalNode != nullptr)
				{
					Vector3Df normal;
					ReadArray(normalNode, "v", (float32*)normal, 3);
					tempVertData.Insert(offset, &normal, sizeof(normal));
					currVertDesc.push_back({ Render::VET_FLOAT32, 3, "NORMAL", 0, offset });
					offset += 12;
				}

				XmlNode* tangentQuatNode = pVertNode->GetFirstNode("tangent_quat");
				if (tangentQuatNode != nullptr)
				{
					Vector4Df  tangentQuat;
					ReadArray(tangentQuatNode, "v", (float32*)tangentQuat, 4);
					tempVertData.Insert(offset, &tangentQuat, sizeof(tangentQuat));
					currVertDesc.push_back({ Render::VET_FLOAT32, 4, "TANGENT_QUAT", 0, offset });
					offset += 16;
				}

				XmlNode* tangentNode = pVertNode->GetFirstNode("tangent");
				if (tangentNode != nullptr)
				{
					Vector3Df tangent;
					ReadArray(tangentNode, "v", (float32*)tangent, 3);
					tempVertData.Insert(offset, &tangent, sizeof(tangent));
					currVertDesc.push_back({ Render::VET_FLOAT32, 3, "TANGENT", 0, offset });
					offset += 12;
				}

				XmlNode* weightNode = pVertNode->GetFirstNode("weight");
				if (weightNode != nullptr)
				{
					Vector4Df weight;
					ReadArray(weightNode, "weight", (float32*)weight, 4);
					tempVertData.Insert(offset, &weight, sizeof(weight));
					currVertDesc.push_back({ Render::VET_FLOAT32, 4, "WEIGHT", 0, offset });
					offset += 16;

					uint8 indices[4] = { 0, 0, 0, 0 };
					ReadArray(weightNode, "joint", (uint8*)&indices, 4);
					tempVertData.Insert(offset, &indices, sizeof(indices));
					currVertDesc.push_back({ Render::VET_UINT8, 4, "INDICES", 0, offset });
					offset += 4;
				}

				XmlNode* pTexNode = pVertNode->GetFirstNode("tex_coord");
				if (pTexNode != nullptr)
				{
					Vector2Df tex;
					ReadArray(pTexNode, "v", (float32*)tex, 2);
					tempVertData.Insert(offset, &tex, sizeof(tex));
					currVertDesc.push_back({ Render::VET_FLOAT32, 2, "TEXCOORD", 0, offset });
					offset += 8;
				}

				if (vertexDesc.size() == 0)
				{
					vertexDesc = currVertDesc;
					stride = offset;

					m_modelData.meshDataList[i].vertDesc = vertexDesc;

					m_modelData.meshDataList[i].vertexData.Alloc(stride * vertNum);
				}
				else
				{
					CompareVertexDesc(vertexDesc, currVertDesc);
				}

				m_modelData.meshDataList[i].vertexData.Insert(j * stride, tempVertData);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//读取索引信息
		//////////////////////////////////////////////////////////////////////////
		XmlNode* pTriListNode = meshNode->GetFirstNode("triangles_chunk");

		uint32 triNum = pTriListNode->GetChildNum();

		uint32 indexNum = triNum * 3;

		m_modelData.meshDataList[i].indexData.Alloc(indexNum * sizeof(uint32));

		for (uint32 j = 0; j < triNum; j++)
		{
			XmlNode* pTriNode = pTriListNode->GetChildNode(j);

			uint32 indices[3] = { 0, 0, 0 };
			ReadArray(pTriNode, "index", indices, 3);
			m_modelData.meshDataList[i].indexData.Insert(j * 3 * sizeof(uint32), indices, sizeof(indices));
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
 		XmlAttribute* mtlIdAttr = meshNode->GetAttribute("mtl_id");
        if(mtlIdAttr != nullptr)
        {
            int32 materId = mtlIdAttr->GetIntValue();
			//m_modelData.meshDataList[i].materialData = m_materialDataList[materId];
        }
		//////////////////////////////////////////////////////////////////////////

		m_modelData.meshDataList[i].drawType = Render::EDT_TRIANGLELIST;
	}
	//////////////////////////////////////////////////////////////////////////
	
	return true;
}

bool XmlSceneImporter::ReadBoneInfo(XmlNode* rootNode)
{
	//读取骨骼信息
	//////////////////////////////////////////////////////////////////////////
	XmlNode* pBoneListNode = rootNode->GetFirstNode("bones_chunk");
	uint32 boneNum = 0;
	if (pBoneListNode != nullptr)
	{
		boneNum = pBoneListNode->GetChildNum();

		m_modelData.boneInfoList.resize(boneNum);

		for (uint32 i = 0; i < boneNum; i++)
		{
			XmlNode* pBoneNode = pBoneListNode->GetChildNode(i);

			XmlAttribute* idAttr = pBoneNode->GetAttribute("id");
			m_modelData.boneInfoList[i].id = idAttr->GetIntValue();

			XmlAttribute* nameAttr = pBoneNode->GetAttribute("name");
			m_modelData.boneInfoList[i].name = nameAttr->GetStringValue();

			XmlAttribute* parentAttr = pBoneNode->GetAttribute("parent");
			m_modelData.boneInfoList[i].parentId = parentAttr->GetIntValue();

			XmlNode* realNode = pBoneNode->GetFirstNode("real");
			XmlNode* dualNode = pBoneNode->GetFirstNode("dual");

			DualQuaternion dq;
			ReadArray(realNode, "v", (float32*)dq.m_real, 4);
			ReadArray(dualNode, "v", (float32*)dq.m_dual, 4);
			m_modelData.boneInfoList[i].bindPose = dq.ToMatrix();
		}
	}
	//////////////////////////////////////////////////////////////////////////	

	return true;
}

bool XmlSceneImporter::ReadAnimation(XmlNode* rootNode)
{
	//读取骨骼动画信息
	//////////////////////////////////////////////////////////////////////////
	XmlNode* pKeyFrameChunkNode = rootNode->GetFirstNode("key_frames_chunk");
	while (pKeyFrameChunkNode != nullptr)
	{
		XmlAttribute* pKeyFrameNameAttr = pKeyFrameChunkNode->GetAttribute("name");
		string animName = pKeyFrameNameAttr->GetStringValue();

		SkelAnimation animInfo;
		animInfo.name = animName;

		XmlAttribute* pKeyFrameNumAttr = pKeyFrameChunkNode->GetAttribute("frames_num");

		uint32 keyFrameNum = pKeyFrameChunkNode->GetChildNum();
		animInfo.frameList.resize(keyFrameNum);
		for (uint32 j = 0; j < keyFrameNum; j++)
		{
			XmlNode* pKeyFrameNode = pKeyFrameChunkNode->GetChildNode(j);

			XmlAttribute* keyFrameIndexAttr = pKeyFrameNode->GetAttribute("joint");
			int32 keyFrameIndex = keyFrameIndexAttr->GetIntValue();

			XmlAttribute* timeAttr = pKeyFrameNode->GetAttribute("time");
			float32 keyFrameTime = timeAttr->GetFloatValue();

			SkelFrame& frameInfo = animInfo.frameList[keyFrameIndex];
			frameInfo.time = keyFrameTime;

			//注意!!!这里我认为keyNum等于boneNum
			uint32 keyNum = pKeyFrameNode->GetChildNum();
			frameInfo.tranList.resize(keyNum);

			for (uint32 k = 0; k < keyNum; k++)
			{
				XmlNode* pKeyNode = pKeyFrameNode->GetChildNode(k);

				XmlAttribute* pKeyIdAttr = pKeyNode->GetAttribute("bone_id");
				int32 boneId = pKeyIdAttr->GetIntValue();

				XmlNode* realNode = pKeyNode->GetFirstNode("real");
				XmlNode* dualNode = pKeyNode->GetFirstNode("dual");

				Quaternion real;
				ReadArray(realNode, "v", (float32*)real, 4);
				Quaternion dual;
				ReadArray(dualNode, "v", (float32*)dual, 4);

				float32 scale = real.DotProduct(real);
				real.Normalize();

				DualQuaternion dq;
				dq.m_real = real;
				dq.m_dual = dual;

				BoneTran& boneTran = frameInfo.tranList[boneId];
				boneTran.bone_id = boneId;
				boneTran.rota = dq.GetRotation();
				boneTran.tran = dq.GetTranslation();
				boneTran.scale = Vector3Df(scale);
			}
		}

		m_modelData.skelAnimList.push_back(animInfo);

		pKeyFrameChunkNode = pKeyFrameChunkNode->GetNextSibling("key_frames_chunk");
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool XmlSceneImporter::CompareVertexDesc(const vector<VertLayoutDesc>& desc1, const vector<VertLayoutDesc>& desc2)
{
	if (desc1.size() != desc2.size()) return false;

	for (uint32 i = 0; i < desc1.size(); i++)
	{
		if (desc1[i].elementType != desc2[i].elementType || desc1[i].elementNum != desc2[i].elementNum 
			|| desc1[i].elementSemantic != desc2[i].elementSemantic || desc1[i].semanticIndex != desc2[i].semanticIndex
			|| desc1[i].elementOffset != desc2[i].elementOffset)
			return false;
	}

	return true;
}