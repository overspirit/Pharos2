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
	XmlDocument* doc = sResMgr->GenerateXmlDocument(file);
	if (!doc->Load()) return false;

	XmlNode* rootNode = doc->GetRootNode();
	ReadMaterialChunk(rootNode);
	ReadMeshChunk(rootNode);
	ReadSkeletonChunk(rootNode);
	ReadAnimationChunk(rootNode);
	ReadModelChunk(rootNode);
	ReadSceneNodeChunk(rootNode);

	return true;
}

bool XmlSceneImporter::ReadSceneNodeChunk(XmlNode* node)
{
	XmlNode* nodeChunkNode = node->GetFirstNode("nodes_chunk");

	uint32 rootNodeNum = nodeChunkNode->GetChildNum();
	m_nodeDataList.resize(rootNodeNum);

	for (uint32 i = 0; i < rootNodeNum; i++)
	{
		XmlNode* nodeNode = nodeChunkNode->GetChildNode(i);

		ReadSceneNode(nodeNode, m_nodeDataList[i]);
	}

	return true;
}

bool XmlSceneImporter::ReadSceneNode(XmlNode* node, SceneNodeData& nodeData)
{
	XmlAttribute* nameAttr = node->GetAttribute("name");
	if (nameAttr != nullptr) nodeData.nodeName = nameAttr->GetStringValue();
	
	XmlAttribute* transAttr = node->GetAttribute("transform");
	if (transAttr != nullptr) nodeData.localTrans = transAttr->GetMatrix4Value();

	XmlAttribute* radiusAttr = node->GetAttribute("bounding_radius");
	if (radiusAttr != nullptr) nodeData.boundRadius = radiusAttr->GetFloatValue();

	for (uint32 i = 0; i < node->GetChildNum(); i++)
	{
		XmlNode* childNode = node->GetChildNode(i);

		const char8* childNodeName = childNode->GetName();
		if (strcmp(childNodeName, "node") == 0)
		{
			nodeData.childData.resize(nodeData.childData.size() + 1);
			
			ReadSceneNode(childNode, *nodeData.childData.rbegin());
		}
		else if (strcmp(childNodeName, "item") == 0)
		{
			XmlAttribute* typeAttr = childNode->GetAttribute("type");
			if (typeAttr == nullptr) continue;

			const char8* itemType = typeAttr->GetStringValue();
			if (strcmp(itemType, "model") == 0)
			{
				XmlAttribute* idAttr = childNode->GetAttribute("id");
				if (idAttr != nullptr)
				{
					nodeData.modelIdList.push_back(idAttr->GetStringValue());
				}
			}
		}
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

bool XmlSceneImporter::ReadModelChunk(XmlNode* node)
{
	XmlNode* modelChunkNode = node->GetFirstNode("models_chunk");
	if (modelChunkNode != nullptr)
	{
		for (uint32 i = 0; i < modelChunkNode->GetChildNum(); i++)
		{
			XmlNode* modelNode = modelChunkNode->GetChildNode(i);
			XmlAttribute* nameAttr = modelNode->GetAttribute("name");
			if (nameAttr != nullptr)
			{
				string modelName = nameAttr->GetStringValue();
				ModelData& modelData = m_modelDataList[modelName];

				ReadModel(modelNode, modelData);				
			}
		}
	}

	return true;
}

bool XmlSceneImporter::ReadModel(XmlNode* node, ModelData& modelData)
{
	if (node == nullptr) return false;

	for (uint32 j = 0; j < node->GetChildNum(); j++)
	{
		XmlNode* itemNode = node->GetChildNode(j);
		XmlAttribute* itemTypeAttr = itemNode->GetAttribute("type");
		XmlAttribute* itemIdAttr = itemNode->GetAttribute("id");

		if (itemTypeAttr != nullptr && itemIdAttr != nullptr)
		{
			string itemType = itemTypeAttr->GetStringValue();
			string itemId = itemIdAttr->GetStringValue();

			if (strcmp(itemType.c_str(), "mesh") == 0)
			{
				ModelData::MeshInfo meshInfo;
				meshInfo.id = itemId;

				for (uint32 k = 0; k < itemNode->GetChildNum(); k++)
				{
					XmlNode* itemChildNode = itemNode->GetChildNode(k);

					XmlAttribute* itemChildAttr = itemChildNode->GetAttribute("type");
					string itemChildType = itemChildAttr->GetStringValue();
					if (strcmp(itemChildType.c_str(), "material") != 0) continue;

					itemChildAttr = itemChildNode->GetAttribute("id");
					string itemChildId = itemChildAttr->GetStringValue();

					meshInfo.materialList.push_back(itemChildId);
				}

				modelData.meshList.push_back(meshInfo);
			}
			else if (strcmp(itemType.c_str(), "skeleton") == 0)
			{
				modelData.skeletonList.push_back(itemId);
			}
			else if (strcmp(itemType.c_str(), "animation") == 0)
			{
				modelData.animList.push_back(itemId);
			}
			else
			{
				assert(false);
			}
		}
	}

	return true;
}

bool XmlSceneImporter::ReadMeshChunk(XmlNode* node)
{	
	//读取网格信息
	//////////////////////////////////////////////////////////////////////////
	XmlNode* meshListNode = node->GetFirstNode("meshes_chunk");
	
	uint32 meshNum = meshListNode->GetChildNum();

	//注意！！根据可以读取顶点的信息修改大小
	MemoryBuffer tempVertData;
	tempVertData.Alloc(128);
	
	for (uint32 i = 0; i < meshNum; i++)
	{
		XmlNode* meshNode = meshListNode->GetChildNode(i);

		//////////////////////////////////////////////////////////////////////////
		XmlAttribute* meshNameAttr = meshNode->GetAttribute("name");
		if (meshNameAttr == nullptr) continue;
		
		const char8* meshName = meshNameAttr->GetStringValue();
		MeshData& meshData = m_meshDataList[meshName];
		meshData.meshName = meshName;
		//////////////////////////////////////////////////////////////////////////

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

					meshData.vertDesc = vertexDesc;

					meshData.vertexData.Alloc(stride * vertNum);
				}
				else
				{
					CompareVertexDesc(vertexDesc, currVertDesc);
				}

				meshData.vertexData.Insert(j * stride, tempVertData);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//读取索引信息
		//////////////////////////////////////////////////////////////////////////
		XmlNode* pTriListNode = meshNode->GetFirstNode("triangles_chunk");

		uint32 triNum = pTriListNode->GetChildNum();

		uint32 indexNum = triNum * 3;

		meshData.indexData.Alloc(indexNum * sizeof(uint32));

		for (uint32 j = 0; j < triNum; j++)
		{
			XmlNode* pTriNode = pTriListNode->GetChildNode(j);

			uint32 indices[3] = { 0, 0, 0 };
			ReadArray(pTriNode, "index", indices, 3);
			meshData.indexData.Insert(j * 3 * sizeof(uint32), indices, sizeof(indices));
		}
		//////////////////////////////////////////////////////////////////////////

		meshData.drawType = Render::EDT_TRIANGLELIST;
	}
	//////////////////////////////////////////////////////////////////////////
	
	return true;
}

bool XmlSceneImporter::ReadSkeletonChunk(XmlNode* node)
{
	//读取骨骼信息
	//////////////////////////////////////////////////////////////////////////

	XmlNode* skeletonListNode = node->GetFirstNode("skeleton_chunk");
	if (skeletonListNode == nullptr) return false;

	uint32 skeletonNum = skeletonListNode->GetChildNum();

	for (uint32 j = 0; j < skeletonNum; j++)
	{
		XmlNode* skeletonNode = skeletonListNode->GetChildNode(j);
		XmlAttribute* skeletonNameAttr = skeletonNode->GetAttribute("name");
		const char8* skeletonName = skeletonNameAttr->GetStringValue();
		SkeletonData& skeletonData = m_skeletonDataList[skeletonName];
		skeletonData.skelName = skeletonName;

		if (skeletonNode != nullptr)
		{
			uint32 boneNum = skeletonNode->GetChildNum();

			skeletonData.boneInfoList.resize(boneNum);

			for (uint32 i = 0; i < boneNum; i++)
			{
				XmlNode* boneNode = skeletonNode->GetChildNode(i);

				XmlAttribute* idAttr = boneNode->GetAttribute("id");
				skeletonData.boneInfoList[i].id = idAttr->GetIntValue();

				XmlAttribute* nameAttr = boneNode->GetAttribute("name");
				skeletonData.boneInfoList[i].name = nameAttr->GetStringValue();

				XmlAttribute* parentAttr = boneNode->GetAttribute("parent");
				skeletonData.boneInfoList[i].parentId = parentAttr->GetIntValue();

				XmlNode* realNode = boneNode->GetFirstNode("real");
				XmlNode* dualNode = boneNode->GetFirstNode("dual");

				DualQuaternion dq;
				ReadArray(realNode, "v", (float32*)dq.m_real, 4);
				ReadArray(dualNode, "v", (float32*)dq.m_dual, 4);
				skeletonData.boneInfoList[i].bindPose = dq.ToMatrix();
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////	

	return true;
}

bool XmlSceneImporter::ReadAnimationChunk(XmlNode* node)
{
	//读取骨骼动画信息
	//////////////////////////////////////////////////////////////////////////

	XmlNode* animListNode = node->GetFirstNode("animation_chunk");
	if (animListNode == nullptr) return false;

	uint32 animNum = animListNode->GetChildNum();
	for(uint32 i = 0; i < animNum; i++)
	{
		XmlNode* animationNode = animListNode->GetChildNode(i);
		if (animationNode == nullptr) continue;

		XmlAttribute* pKeyFrameNameAttr = animationNode->GetAttribute("name");
		string animName = pKeyFrameNameAttr->GetStringValue();

		//XmlAttribute* pKeyFrameNumAttr = animationNode->GetAttribute("frames_num");

		uint32 keyFrameNum = animationNode->GetChildNum();
		
		SkelAnimation& animData = m_animDataList[animName];
		animData.name = animName;
		animData.frameList.resize(keyFrameNum);

		for (uint32 j = 0; j < keyFrameNum; j++)
		{
			XmlNode* pKeyFrameNode = animationNode->GetChildNode(j);

			XmlAttribute* keyFrameIndexAttr = pKeyFrameNode->GetAttribute("joint");
			int32 keyFrameIndex = keyFrameIndexAttr->GetIntValue();

			XmlAttribute* timeAttr = pKeyFrameNode->GetAttribute("time");
			float32 keyFrameTime = timeAttr->GetFloatValue();

			SkelFrame& frameInfo = animData.frameList[keyFrameIndex];
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
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool XmlSceneImporter::ReadMaterialChunk(XmlNode* node)
{
	//读取材质信息
	//////////////////////////////////////////////////////////////////////////
	XmlNode* materialListNode = node->GetFirstNode("materials_chunk");

	uint32 materialNum = materialListNode->GetChildNum();

	for (uint32 i = 0; i < materialNum; i++)
	{
		XmlNode* materialNode = materialListNode->GetChildNode(i);
		XmlAttribute* materialNameAttr = materialNode->GetAttribute("name");
		if (materialNameAttr != nullptr)
		{
			const char8* materialName = materialNameAttr->GetStringValue();
			MaterialData& materialData = m_materialDataList[materialName];
			materialData.materialName = materialName;

			XmlAttribute* materialTechAttr = materialNode->GetAttribute("technique");
			if (materialTechAttr != nullptr) materialData.techName = materialTechAttr->GetStringValue();

			for (uint32 j = 0; j < materialNode->GetChildNum(); j++)
			{
				XmlNode* childNode = materialNode->GetChildNode(j);
				XmlAttribute* nameAttr = childNode->GetAttribute("name");
				XmlAttribute* valueAttr = childNode->GetAttribute("value");
				const char8* childName = childNode->GetName();

				if (strcmp(childName, "variable") == 0)
				{
					if (nameAttr != nullptr && valueAttr != nullptr)
					{
						const char8* varName = nameAttr->GetStringValue();
						const char8* varValue = valueAttr->GetStringValue();
						materialData.varList[varName] = varValue;
					}
				}
				else if (strcmp(childName, "state") == 0)
				{
					if (nameAttr != nullptr && valueAttr != nullptr)
					{
						const char8* stateName = nameAttr->GetStringValue();
						const char8* stateValue = valueAttr->GetStringValue();

						materialData.stateList[stateName] = stateValue;
					}
				}
				else if (strcmp(childName, "sampler") == 0)
				{
					if (nameAttr != nullptr)
					{
						const char8* sampleName = nameAttr->GetStringValue();
						SamplerData& sampleData = materialData.samplerDataList[sampleName];
						sampleData.samplerName = sampleName;

						XmlNode* texNode = childNode->GetFirstNode("texture");						
						XmlAttribute* pathAttr = texNode->GetAttribute("path");
						if (pathAttr != nullptr)
						{
							sampleData.samplerName = sampleName;
							sampleData.texPath = pathAttr->GetStringValue();
						}
					}
				}
			}
		}
	}

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