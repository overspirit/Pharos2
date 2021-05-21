#include "PreCompile.h"
#include "Pharos.h"

XmlSceneImporter::XmlSceneImporter()
{

}

XmlSceneImporter::~XmlSceneImporter(void)
{

}

bool XmlSceneImporter::LoadSceneFile(const char8* file)
{
	XmlDocument* doc = sResMgr->GenerateXmlDocument(file);
	if (doc == nullptr) return false;

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
                modelData.name = modelName;

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
		meshData.vertCount = vertNum;

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
				currVertDesc.push_back({ VET_FLOAT32, 3, VAL_POSITION, offset, 0 });
				offset += 12;
                
                XmlAttribute* colorAttr = pVertNode->GetAttribute("color");
                if (colorAttr != nullptr)
                {
                    Color4 color = colorAttr->GetColorValue();
                    tempVertData.Insert(offset, &color, sizeof(color));
                    currVertDesc.push_back({ VET_UNORM8, 4, VAL_COLOR, offset, 0 });
                    offset += 4;
                }
                
				XmlNode* normalNode = pVertNode->GetFirstNode("normal");
				if (normalNode != nullptr)
				{
					Vector3Df normal;
					ReadArray(normalNode, "v", (float32*)normal, 3);
					tempVertData.Insert(offset, &normal, sizeof(normal));
					currVertDesc.push_back({ VET_FLOAT32, 3, VAL_NORMAL, offset, 0 });
					offset += 12;
				}

				XmlNode* tangentQuatNode = pVertNode->GetFirstNode("tangent_quat");
				if (tangentQuatNode != nullptr)
				{
					Vector4Df  tangentQuat;
					ReadArray(tangentQuatNode, "v", (float32*)tangentQuat, 4);
					tempVertData.Insert(offset, &tangentQuat, sizeof(tangentQuat));
					currVertDesc.push_back({ VET_FLOAT32, 4, VAL_TANGENT_QUAT, offset, 0 });
					offset += 16;
				}

				XmlNode* tangentNode = pVertNode->GetFirstNode("tangent");
				if (tangentNode != nullptr)
				{
					Vector3Df tangent;
					ReadArray(tangentNode, "v", (float32*)tangent, 3);
					tempVertData.Insert(offset, &tangent, sizeof(tangent));
					currVertDesc.push_back({ VET_FLOAT32, 3, VAL_TANGENT, offset, 0 });
					offset += 12;
				}

				XmlNode* weightNode = pVertNode->GetFirstNode("weight");
				if (weightNode != nullptr)
				{
					Vector4Df weight;
					ReadArray(weightNode, "weight", (float32*)weight, 4);
					tempVertData.Insert(offset, &weight, sizeof(weight));
					currVertDesc.push_back({ VET_FLOAT32, 4, VAL_WEIGHT, offset, 0 });
					offset += 16;

					uint8 indices[4] = { 0, 0, 0, 0 };
					ReadArray(weightNode, "joint", (uint8*)&indices, 4);
					tempVertData.Insert(offset, &indices, sizeof(indices));
					currVertDesc.push_back({ VET_UINT8, 4, VAL_INDICES, offset, 0 });
					offset += 4;
				}

				XmlNode* pTexNode = pVertNode->GetFirstNode("tex_coord");
				if (pTexNode != nullptr)
				{
					Vector2Df tex;
					ReadArray(pTexNode, "v", (float32*)tex, 2);
					tempVertData.Insert(offset, &tex, sizeof(tex));
					currVertDesc.push_back({ VET_FLOAT32, 2, VAL_TEXCOORD0, offset, 0 });
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
		meshData.faceCount = triNum;

		uint32 indexNum = triNum * 3;

		meshData.indexData.Alloc(indexNum * sizeof(uint32));

		for (uint32 j = 0; j < triNum; j++)
		{
			XmlNode* pTriNode = pTriListNode->GetChildNode(j);

			uint32 indices[3] = { 0, 0, 0 };	
			ReadArray(pTriNode, "index", indices, 3);
			//uint32 temp = indices[1];
			//indices[1] = indices[2];
			//indices[2] = temp;
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
	for (uint32 i = 0; i < animNum; i++)
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
        XmlAttribute* materialTechAttr = materialNode->GetAttribute("technique");
        
        if (materialNameAttr == nullptr || materialTechAttr == nullptr) continue;
        
        const char8* materialName = materialNameAttr->GetStringValue();
        MaterialData& materialData = m_materialDataList[materialName];
        materialData.materialName = materialName;
        materialData.techName = materialTechAttr->GetStringValue();
        materialData.lightDir = Vector3Df(0.577350259f, -0.577350259f, 0.577350259f);
        materialData.lightColor = 0xFFFFFFFF;
        materialData.mateialColor = 0xFF3F3F3F;
        materialData.ambientColor = 0xFFFFFFFF;
        materialData.ambientAlbedo = 0.3f;
        materialData.diffuseColor = 0xFFFFFFFF;
        materialData.diffuseAlbedo = 0.4f;
        materialData.specularColor = 0xFFFFFFFF;
        materialData.specularAlbedo = 0.3f;
        materialData.specularPow = 16.0f;
        materialData.transparent = false;
        materialData.forceDepth = false;
        materialData.cullBack = false;
        materialData.clockWise = true;
        
        XmlAttribute* materialColorAttr = materialNode->GetAttribute("color");
        if (materialColorAttr != NULL)
        {
            materialData.mateialColor = materialColorAttr->GetColorValue();
        }
        
        for (uint32 j = 0; j < materialNode->GetChildNum(); j++)
        {
            XmlNode* childNode = materialNode->GetChildNode(j);
            const char8* childName = childNode->GetName();
                        
            if (strcmp(childName, "light") == 0)
            {
                Vector3Df lightDir;
                if (ReadArray(childNode, "dir", (float32*)lightDir, 3))
                {
                    materialData.lightDir = lightDir;
                }
                
                XmlAttribute* lightColorAttr = childNode->GetAttribute("value");
                if (lightColorAttr != NULL)
                {
                    materialData.lightColor = lightColorAttr->GetColorValue();
                }
            }
            else if (strcmp(childName, "ambient") == 0)
            {
                XmlAttribute* ambientColorAttr = childNode->GetAttribute("value");
                if (ambientColorAttr != NULL)
                {
                    materialData.ambientColor = ambientColorAttr->GetColorValue();
                }
                
                XmlAttribute* ambientAlbedoAttr = childNode->GetAttribute("albedo");
                if (ambientAlbedoAttr != NULL)
                {
                    materialData.ambientAlbedo = ambientAlbedoAttr->GetFloatValue();
                }
                
            }
            else if (strcmp(childName, "diffuse") == 0)
            {
                XmlAttribute* diffuseColorAttr = childNode->GetAttribute("value");
                if (diffuseColorAttr != NULL)
                {
                    materialData.diffuseColor = diffuseColorAttr->GetColorValue();
                }
                
                XmlAttribute* diffuseAlbedoAttr = childNode->GetAttribute("albedo");
                if (diffuseAlbedoAttr != NULL)
                {
                    materialData.diffuseAlbedo = diffuseAlbedoAttr->GetFloatValue();
                }
            }
            else if (strcmp(childName, "specular") == 0)
            {
                XmlAttribute* specularColorAttr = childNode->GetAttribute("value");
                if (specularColorAttr != NULL)
                {
                    materialData.specularColor = specularColorAttr->GetColorValue();
                }
                
                XmlAttribute* specularAlbedoAttr = childNode->GetAttribute("albedo");
                if (specularAlbedoAttr != NULL)
                {
                    materialData.specularAlbedo = specularAlbedoAttr->GetFloatValue();
                }
                
                XmlAttribute* specularPowAttr = childNode->GetAttribute("pow");
                if (specularPowAttr != NULL)
                {
                    materialData.specularPow = specularPowAttr->GetFloatValue();
                }
            }
            else if (strcmp(childName, "state") == 0)
            {
                XmlAttribute* transparentAttr = childNode->GetAttribute("transparent");
                if (transparentAttr != NULL)
                {
                    materialData.transparent = transparentAttr->GetBoolValue();
                }
                
                XmlAttribute* forceDepthAttr = childNode->GetAttribute("force_depth");
                if (forceDepthAttr != NULL)
                {
                    materialData.forceDepth = forceDepthAttr->GetBoolValue();
                }
                
                XmlAttribute* cullBackAttr = childNode->GetAttribute("cull_back");
                if (transparentAttr != NULL)
                {
                    materialData.cullBack = cullBackAttr->GetBoolValue();
                }
                
                XmlAttribute* clockWiseAttr = childNode->GetAttribute("clock_wise");
                if (transparentAttr != NULL)
                {
                    materialData.clockWise = clockWiseAttr->GetBoolValue();
                }
                
            }
			else if (strcmp(childName, "sampler") == 0)
			{
				XmlAttribute* typeAttr = childNode->GetAttribute("type");
				if (typeAttr != nullptr)
				{
					XmlNode* texNode = childNode->GetFirstNode("texture");
					if (texNode != NULL)
					{
						XmlAttribute* pathAttr = texNode->GetAttribute("path");						
						if (pathAttr != NULL)
						{
							SamplerData sampleData;
							sampleData.samplerType = typeAttr->GetStringValue();
							sampleData.texPath = pathAttr->GetStringValue();
							//sampleData.stateDesc = xxx;
							materialData.samplerDataList.push_back(sampleData);
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
			|| desc1[i].elementLocation != desc2[i].elementLocation || desc1[i].layoutIndex != desc2[i].layoutIndex
			|| desc1[i].elementOffset != desc2[i].elementOffset)
			return false;
	}

	return true;
}
